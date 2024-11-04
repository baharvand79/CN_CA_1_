#include "webrtc.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include <QtEndian>
#include <QJsonDocument>

RTPHeader WebRTC::createRTPHeader(uint8_t payloadType, uint16_t sequenceNumber, uint32_t timestamp, uint32_t ssrc) {
    RTPHeader header;
    header.marker = 0;
    header.payloadType = payloadType;
    header.sequenceNumber = qToBigEndian(sequenceNumber);
    header.timestamp = qToBigEndian(getCurrentTimeStamp());
    header.ssrc = qToBigEndian(ssrc);

    Q_EMIT debugMessage("[WebRTC] Created RTP header: Version: " + QString::number(2) + ", Payload Type: " + QString::number(header.payloadType) + ", Sequence Number: " + QString::number(header.sequenceNumber) + ", Timestamp: " + QString::number(header.timestamp));

    return header;
}



std::string peerConnectionStateToString(rtc::PeerConnection::State state) {
    switch (state) {
    case rtc::PeerConnection::State::New: return "New";
    case rtc::PeerConnection::State::Connecting: return "Connecting";
    case rtc::PeerConnection::State::Connected: return "Connected";
    case rtc::PeerConnection::State::Disconnected: return "Disconnected";
    case rtc::PeerConnection::State::Failed: return "Failed";
    case rtc::PeerConnection::State::Closed: return "Closed";
    default: return "Unknown State";
    }
}

WebRTC::WebRTC(QObject *parent)
    : QObject{parent},
      webSocket(new QWebSocket()),
      localSDP("")
{
    Q_EMIT debugMessage("[WebRTC] Constructor called: Initialization completed.");
    peerIsOfferer = false;
}


bool WebRTC::getIsOfferer(){
    return peerIsOfferer;
}
void WebRTC::setId(QString id) {
    peerId = id;
    Q_EMIT debugMessage("[WebRTC] Peer ID set to: " + peerId);
}

void WebRTC::setTargetId(QString id) {
    targetId = id;
    Q_EMIT debugMessage("[WebRTC] Target ID set to: " + targetId);
}

void WebRTC::init() {
    rtc::InitLogger(rtc::LogLevel::Debug);
    Q_EMIT debugMessage("[WebRTC] Initialization started.");

    config.iceServers.emplace_back("stun:stun.l.google.com:19302");
    config.iceServers.emplace_back("turn:openrelayproject:openrelayproject@openrelay.metered.ca:80");

    peerConnection = std::make_shared<rtc::PeerConnection>(config);
    Q_EMIT debugMessage("[WebRTC] ICE servers configured and peer connection created.");

    auto audio = rtc::Description::Audio("Audio",
                                         this->peerIsOfferer?
                                             rtc::Description::Direction::SendOnly : rtc::Description::Direction::RecvOnly);

    audio.setBitrate(48000);
    audio.addOpusCodec(111);
    audio.addSSRC(2, "shakiba");

    audioTrack = peerConnection->addTrack(audio);

    audioTrack->onMessage([this](rtc::message_variant data){
        Q_EMIT debugMessage("[WebRTC] Data is received in the onMessage.");

        QByteArray audioData;

        if (std::holds_alternative<std::vector<std::byte>>(data)) {
            const auto& rawData = std::get<std::vector<std::byte>>(data);

            if (rawData.size() > sizeof(RTPHeader)) {
                audioData = QByteArray(reinterpret_cast<const char*>(rawData.data() + sizeof(RTPHeader)),
                                       rawData.size() - sizeof(RTPHeader));
                Q_EMIT audioDataReceived(audioData);
                Q_EMIT debugMessage("[WebRTC] Received audio packet and stripped RTP header. Data size: "
                                    + QString::number(audioData.size()) + " bytes.");
            } else {
                Q_EMIT debugMessage("[WebRTC] Received packet too small for RTP header removal.");
            }
        }

        audioTrack->onOpen([this](){
            Q_EMIT debugMessage("[WebRTC] Track is open now ================================================");
        });

        Q_EMIT debugMessage("[WebRTC] Audio track added to peer connection.");

    });
}

void WebRTC::onAudioCaptured(const QByteArray &audioData) {
    Q_EMIT debugMessage("[WebRTC] onAudioCaptured.");
    if (audioTrack) {
        std::vector<std::byte> byteData(audioData.size());
        std::memcpy(byteData.data(), audioData.data(), audioData.size());
        audioTrack->send(byteData);
        Q_EMIT debugMessage("[WebRTC] Audio data sent. Size: " + QString::number(audioData.size()) + " bytes.");
    }
}

bool WebRTC::getPeerIsOfferer() const
{
    return peerIsOfferer;
}

void WebRTC::setPeerIsOfferer(bool newPeerIsOfferer)
{
    if (peerIsOfferer == newPeerIsOfferer)
        return;
    peerIsOfferer = newPeerIsOfferer;
    emit peerIsOffererChanged();
}

void WebRTC::resetPeerIsOfferer()
{
    setPeerIsOfferer({}); // TODO
}


void WebRTC::callOnRun() {
    Q_EMIT debugMessage("[WebRTC] callOnRun initiated.");

    peerConnection->onLocalDescription([this](rtc::Description description) {
        Q_EMIT debugMessage("[WebRTC] Local SDP offer being created.");
        Q_EMIT localDescriptionGenerated(QString::fromStdString(description));
        isLocalDescriptionGenerated = true;
        localSDP = QString::fromStdString(description);
        // Change the version line (v=0 to v=1)
        QStringList sdpLines = localSDP.split('\n');
        for (int i = 0; i < sdpLines.size(); ++i) {
            if (sdpLines[i].startsWith("v=")) {
                if(peerIsOfferer){
                    sdpLines[i] = "v=1";
                } else {
                    sdpLines[i] = "v=2";
                }
                break;
            }
        }
        localSDP = sdpLines.join('\n');
        Q_EMIT debugMessage("[WebRTC] Local SDP generated and emitted\n"+localSDP);
    });



    peerConnection->onLocalCandidate([this](rtc::Candidate candidate) {
        QString candidateStr = QString::fromStdString(candidate);
        localIceCandidates.append(candidateStr);
        Q_EMIT localCandidateGenerated(candidateStr);
        Q_EMIT debugMessage("[WebRTC] Local ICE candidate generated and emitted: " + candidateStr);
    });


    peerConnection->onTrack([this](std::shared_ptr<rtc::Track> track) {
        //        Q_EMIT debugMessage("[WebRTC] &&&&&&&&&&&&&&&&&&&&Audio track received from remote peer.&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
        //        track->onMessage([this](rtc::message_variant data){
        //            Q_EMIT debugMessage("[WebRTC] ONMESSAGE================================================");
        //        });
    });





    peerConnection->onStateChange([this](rtc::PeerConnection::State state) {
        QString stateStr = QString::fromStdString(peerConnectionStateToString(state));
        Q_EMIT debugMessage("[WebRTC] PeerConnection state changed: " + stateStr);
        if (state == rtc::PeerConnection::State::Failed) {
            Q_EMIT debugMessage("[WebRTC] Warning: Connection failed; a TURN server might be required.");
        }
        if (state == rtc::PeerConnection::State::Connected) {
            Q_EMIT debugMessage("[WebRTC] Success: Connection is set!");
            if(audioTrack->isOpen()){
                Q_EMIT debugMessage("[WebRTC] Track is open.");
            } else {
                Q_EMIT debugMessage("[WebRTC] Track is not open.");
            }

            Q_EMIT peerIsConnected();
        }
    });

    peerConnection->onGatheringStateChange([this](rtc::PeerConnection::GatheringState state) {
        if (state == rtc::PeerConnection::GatheringState::Complete) {
            Q_EMIT gatheringCompleted();
            Q_EMIT debugMessage("[WebRTC] ICE gathering completed successfully.");
        }
    });
}

// Function to convert WebSocket state to string
QString websocketStateToString(QAbstractSocket::SocketState state) {
    switch (state) {
    case QAbstractSocket::UnconnectedState: return "UnconnectedState";
    case QAbstractSocket::HostLookupState: return "HostLookupState";
    case QAbstractSocket::ConnectingState: return "ConnectingState";
    case QAbstractSocket::ConnectedState: return "ConnectedState";
    case QAbstractSocket::BoundState: return "BoundState";
    case QAbstractSocket::ListeningState: return "ListeningState";
    case QAbstractSocket::ClosingState: return "ClosingState";
    default: return "Unknown State";
    }
}

// Example usage in your WebRTC class
void WebRTC::checkWebSocketState() {
    QString stateString = websocketStateToString(webSocket->state());
    Q_EMIT debugMessage("[WebRTC] Current WebSocket state: " + stateString);
}

void WebRTC::registerClient() {
    if (!peerId.isEmpty()) {
        QJsonObject registrationMessage;
        registrationMessage["type"] = "register";
        registrationMessage["id"] = peerId;

        webSocket->sendTextMessage(QJsonDocument(registrationMessage).toJson());
        Q_EMIT debugMessage("[WebRTC] Registration message sent for Peer ID: " + peerId);
        checkWebSocketState();
        Q_EMIT clientIsRegistered();
        isClientRegistered = true;
    } else {
        Q_EMIT debugMessage("[WebRTC] Error: Peer ID is empty; registration skipped.");
    }
}

void WebRTC::connectToSignalingServer() {
    Q_EMIT debugMessage("[WebRTC] Attempting to connect to signaling server at ws://localhost:3000.");
    webSocket->open(QUrl("ws://localhost:3000"));

    connect(webSocket.get(), &QWebSocket::connected, this, &WebRTC::onSignalingServerConnected);
    connect(webSocket.get(), &QWebSocket::disconnected, this, &WebRTC::onSignalingServerDisconnected);
    connect(webSocket.get(), &QWebSocket::textMessageReceived, this, &WebRTC::onSignalingMessageReceived);
}


void WebRTC::sendOffer() {
    peerIsOfferer = true;
    Q_EMIT debugMessage("[WebRTC] The peer is set to offerer.");
    QJsonObject offerMessage;
    offerMessage["type"] = "offer";
    offerMessage["id"] = peerId;
    offerMessage["targetId"] = targetId;
    QStringList sdpLines = localSDP.split('\n');
    for (int i = 0; i < sdpLines.size(); ++i) {
        if (sdpLines[i].startsWith("v=")) {
            if(peerIsOfferer){
                sdpLines[i] = "v=1";
            } else {
                sdpLines[i] = "v=2";
            }
            break;
        }
    }
    localSDP = sdpLines.join('\n');
    offerMessage["sdp"] = localSDP;

    if (webSocket->state() == QAbstractSocket::ConnectedState) {
        webSocket->sendTextMessage(QJsonDocument(offerMessage).toJson());
        Q_EMIT debugMessage("[WebRTC] SDP Offer sent to signaling server: " + localSDP);
    } else {
        Q_EMIT debugMessage("[WebRTC] Error: WebSocket not connected; cannot send offer.");
    }
}

void WebRTC::sendAnswer(){
    QJsonObject answerMessage;
    answerMessage["type"] = "answer";
    answerMessage["id"] = peerId;
    answerMessage["targetId"] = targetId;
    answerMessage["sdp"] = localSDP;

    if (webSocket->state() == QAbstractSocket::ConnectedState) {
        webSocket->sendTextMessage(QJsonDocument(answerMessage).toJson());
        Q_EMIT debugMessage("[WebRTC] SDP Answer sent to signaling server: " + localSDP);
    } else {
        Q_EMIT debugMessage("[WebRTC] Error: WebSocket not connected; cannot send answer.");
    }
}

void WebRTC::onSignalingServerConnected() {
    Q_EMIT debugMessage("[WebRTC] Successfully connected to signaling server.");
    registerClient();
}

void WebRTC::onSignalingServerDisconnected() {
    Q_EMIT debugMessage("[WebRTC] Disconnected from signaling server.");
}

void WebRTC::onSignalingMessageReceived(const QString &message) {
    Q_EMIT debugMessage("[WebRTC] Signaling message received from server: " + message);

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObj = doc.object();
    QString type = jsonObj["type"].toString();

    if (type == "offer") {
        QString sdp = jsonObj["sdp"].toString();
        remoteSDP = sdp;
        Q_EMIT debugMessage("[WebRTC] Received offer with SDP: " + sdp);
        targetId = jsonObj["id"].toString();
        peerConnection->setRemoteDescription(rtc::Description(sdp.toStdString(), type.toStdString()));
        Q_EMIT debugMessage("[WebRTC] Remote SDP set for offer.");
        createAnswer();
        sendAnswer();

    } else if (type == "answer") {
        QString sdp = jsonObj["sdp"].toString();
        remoteSDP = sdp;
        peerConnection->setRemoteDescription(rtc::Description(sdp.toStdString(), type.toStdString()));
        Q_EMIT debugMessage("[WebRTC] Remote SDP set for answer.");
    } else if (type == "ice-candidate") {
        QString candidate = jsonObj["candidate"].toString();
        remoteIceCandidates.append(candidate);
        peerConnection->addRemoteCandidate(rtc::Candidate(candidate.toStdString()));
        Q_EMIT debugMessage("[WebRTC] Remote ICE candidate added: " + candidate);
    } else {
        Q_EMIT debugMessage("[WebRTC] Warning: Unrecognized signaling message type: " + type);
    }
}

void WebRTC::createAnswer() {
    Q_EMIT debugMessage("[WebRTC] Creating new SDP answer.");

}

void WebRTC::createOffer() {
    Q_EMIT debugMessage("[WebRTC] Creating new SDP offer.");
    peerConnection->setLocalDescription(rtc::Description::Type::Offer);
}

void WebRTC::addRemoteCandidate(const QString& candidate) {
    Q_EMIT debugMessage("[WebRTC] Adding remote ICE candidate: " + candidate);
    peerConnection->addRemoteCandidate(rtc::Candidate(candidate.toStdString()));
    Q_EMIT debugMessage("[WebRTC] Remote ICE candidate added.");
}

void WebRTC::sendTrack(const QByteArray &audioData) {
    Q_EMIT debugMessage("[WebRTC] Preparing to send track data. Size: " + QString::number(audioData.size()) + " bytes.");

    if (audioTrack) {
        // Create the RTP header
        RTPHeader rtpHeader = createRTPHeader(111, sequenceNumber++, timestamp, ssrc);

        // Prepare the RTP packet
        size_t rtpPacketSize = sizeof(RTPHeader) + audioData.size();
        std::vector<std::byte> rtpPacket(rtpPacketSize);

        std::memcpy(rtpPacket.data(), &rtpHeader, sizeof(RTPHeader));
        std::memcpy(rtpPacket.data() + sizeof(RTPHeader), audioData.data(), audioData.size());

        if (audioTrack->send(rtpPacket)) {
            Q_EMIT debugMessage("***************************************************************");
            Q_EMIT debugMessage("[WebRTC] RTP packet sent successfully. Size: " + QString::number(rtpPacket.size()) + " bytes. Sequence Number: " + QString::number(rtpHeader.sequenceNumber));
        } else {
            Q_EMIT debugMessage("[WebRTC] Error: Failed to send RTP packet.");
        }

        timestamp += 160; // Assuming 8000 Hz audio, 20 ms per packet = 160 samples
        Q_EMIT debugMessage("[WebRTC] Updated timestamp for next packet: " + QString::number(timestamp));
    } else {
        Q_EMIT debugMessage("[WebRTC] Error: Audio track is not initialized; cannot send audio data.");
    }
}

