#include "webrtc.h"
#include <QtEndian>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/// ** explanation
/// $$ later
/// // TA's comment
static_assert(true);

#pragma pack(push, 1)
struct RtpHeader {
    uint8_t first;
    uint8_t marker:1;
    uint8_t payloadType:7;
    uint16_t sequenceNumber;
    uint32_t timestamp;
    uint32_t ssrc;
};
#pragma pack(pop)


WebRTC::WebRTC(QObject *parent)
    : QObject{parent},
    m_audio("Audio")
{
    connect(this, &WebRTC::gatheringComplited, [this] (const QString &peerID) {

        m_localDescription = descriptionToJson(m_peerConnections[peerID]->localDescription().value());
        Q_EMIT localDescriptionGenerated(peerID, m_localDescription);

        if (m_isOfferer)
            Q_EMIT this->offerIsReady(peerID, m_localDescription);
        else
            Q_EMIT this->answerIsReady(peerID, m_localDescription);
    });
}

WebRTC::~WebRTC()
{}


///**
// * ====================================================
// * ================= public methods ===================
// * ====================================================
// */


void WebRTC::init(const QString &id, bool isOfferer)
{
//    // Initialize WebRTC using libdatachannel library

//    // Create an instance of rtc::Configuration to Set up ICE configuration

//    // Add a STUN server to help peers find their public IP addresses

//    // Add a TURN server for relaying media if a direct connection can't be established

//    // Set up the audio stream configuration

    qDebug() << "Initializing WebRTC for peer:" << id << "as" << (isOfferer ? "Offerer" : "Answerer");

    m_localId = id;
    m_isOfferer = isOfferer;

    m_config.iceServers.push_back({"stun:stun.l.google.com:19302"});  // STUN server
    m_config.iceServers.push_back({"turn:turnserver.com:3478", "username", "password"});  // TURN server if necessary
    // $$ DON'T FORGET TO FIX IT!

    m_audio = rtc::Description::Audio();
    qDebug() << "WebRTC initialized with STUN and TURN servers.";

}

void WebRTC::addPeer(const QString &peerId)
{

    qDebug() << "Adding peer:" << peerId;
//     // Create and add a new peer connection

    auto newPeer = std::make_shared<rtc::PeerConnection>(m_config);
    m_peerConnections.insert(peerId, newPeer);

//    // Set up a callback for when the local description is generated

    // **  it's processed or emitted as per your application's logic
    // **  handle the local description and emit it based on the peer's role.
    newPeer->onLocalDescription([this, peerId](const rtc::Description &description) {
        // The local description should be emitted using the appropriate signals based on the peer's role (offerer or answerer)
        qDebug() << "Local description generated for peer:" << peerId;
        QString sdp = QString::fromStdString(description.generateSdp());
         qDebug() << "SDP for peer" << peerId << ":" << sdp;

        // Emit signal to indicate that the local description has been generated
        Q_EMIT localDescriptionGenerated(peerId, sdp);

        // Check the type of description (Offer or Answer)
        if (description.type() == rtc::Description::Type::Offer) {
            emit offerIsReady(peerId, sdp);
            qDebug() << "Offer is ready for peer:" << peerId;

        } else if (description.type() == rtc::Description::Type::Answer) {
            emit answerIsReady(peerId, sdp);
            qDebug() << "Answer is ready for peer:" << peerId;
        }


    });


//    // Set up a callback for handling local ICE candidates
//    newPeer->onLocalCandidate([this, peerId](rtc::Candidate candidate) {
//        // Emit the local candidates using the localCandidateGenerated signal

//    });



//    // Set up a callback for when the state of the peer connection changes
//    newPeer->onStateChange([this, peerId](rtc::PeerConnection::State state) {
//        // Handle different states like New, Connecting, Connected, Disconnected, etc.

//    });



//    // Set up a callback for monitoring the gathering state
//    newPeer->onGatheringStateChange([this, peerId](rtc::PeerConnection::GatheringState state) {
//        // When the gathering is complete, emit the gatheringComplited signal

//    });

//    // Set up a callback for handling incoming tracks
//    newPeer->onTrack([this, peerId] (std::shared_ptr<rtc::Track> track) {
//        // handle the incoming media stream, emitting the incommingPacket signal if a stream is received

//    });

//    // Add an audio track to the peer connection


}

//// Set the local description for the peer's connection
void WebRTC::generateOfferSDP(const QString &peerId)
{
    qDebug() << "Generating SDP offer for peer:" << peerId;

    if (m_peerConnections.contains(peerId)) {
        m_peerConnections[peerId]->setLocalDescription(rtc::Description::Type::Offer);
        qDebug() << "Offer SDP generated for peer:" << peerId;
    } else {
        qWarning() << "Peer not found:" << peerId;
    }

}

//// Generate an answer SDP for the peer
void WebRTC::generateAnswerSDP(const QString &peerId)
{

}

//// Add an audio track to the peer connection
void WebRTC::addAudioTrack(const QString &peerId, const QString &trackName)
{
//    // Add an audio track to the peer connection

//    // Handle track events

//    track->onMessage([this, peerId] (rtc::message_variant data) {

//    });

//    track->onFrame([this] (rtc::binary frame, rtc::FrameInfo info) {

//    });

}

//// Sends audio track data to the peer

void WebRTC::sendTrack(const QString &peerId, const QByteArray &buffer)
{

    // Create the RTP header and initialize an RtpHeader struct


    // Create the RTP packet by appending the RTP header and the payload buffer


    // Send the packet, catch and handle any errors that occur during sending

}


///**
// * ====================================================
// * ================= public slots =====================
// * ====================================================
// */

//// Set the remote SDP description for the peer that contains metadata about the media being transmitted
void WebRTC::setRemoteDescription(const QString &peerID, const QString &sdp)
{

}

//// Add remote ICE candidates to the peer connection
void WebRTC::setRemoteCandidate(const QString &peerID, const QString &candidate, const QString &sdpMid)
{

}


///*
// * ====================================================
// * ================= private methods ==================
// * ====================================================
// */

//// Utility function to read the rtc::message_variant into a QByteArray
QByteArray WebRTC::readVariant(const rtc::message_variant &data)
{

}

//// Utility function to convert rtc::Description to JSON format
QString WebRTC::descriptionToJson(const rtc::Description &description) {

}

//// Retrieves the current bit rate
int WebRTC::bitRate() const
{

}

//// Set a new bit rate and emit the bitRateChanged signal
void WebRTC::setBitRate(int newBitRate)
{

}

//// Reset the bit rate to its default value
void WebRTC::resetBitRate()
{

}

//// Sets a new payload type and emit the payloadTypeChanged signal
void WebRTC::setPayloadType(int newPayloadType)
{

}

//// Resets the payload type to its default value
void WebRTC::resetPayloadType()
{

}

//// Retrieve the current SSRC value
rtc::SSRC WebRTC::ssrc() const
{

}

//// Set a new SSRC and emit the ssrcChanged signal
void WebRTC::setSsrc(rtc::SSRC newSsrc)
{

}

//// Reset the SSRC to its default value
void WebRTC::resetSsrc()
{

}

//// Retrieve the current payload type
int WebRTC::payloadType() const
{

}


///**
// * ====================================================
// * ================= getters setters ==================
// * ====================================================
// */

bool WebRTC::isOfferer() const
{

}

void WebRTC::setIsOfferer(bool newIsOfferer)
{

}

void WebRTC::resetIsOfferer()
{

}


