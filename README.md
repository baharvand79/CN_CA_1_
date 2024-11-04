# Computer Network, Assignment 1
# Implementing a distributed voice call using WebRTC
Fatemeh (Shakiba) Baharvand
## Explanation
### WebRTC
- **WebRTC** stands for Web Real-Time Communication, an open-source technology.
- It allows real-time video, audio, and data sharing directly between web browsers and mobile applications.
- WebRTC creates a peer-to-peer (P2P) connection, so data can be sent directly between users.
- There is no need for intermediary servers to relay data, which reduces delays and improves performance.
- Applications of WebRTC include video conferencing, online gaming, remote collaboration, and telemedicine.
- WebRTC’s main features are:
  - Low-latency P2P connectivity that allows real-time communication.
  - Cross-platform compatibility with major browsers and mobile platforms.
  - End-to-end encryption for secure data sharing.
  - Support for audio and video capture and streaming.
- Implementing WebRTC requires several steps, including:
  - Setting up signaling to establish the initial connection between users.
  - Managing network issues like firewalls or NAT (Network Address Translation) that can block direct connections.
  - Handling codec compatibility for audio and video on different devices.
- Despite its challenges, WebRTC’s ecosystem has tools and libraries like to make development easier.
- With WebRTC, developers can build dynamic, real-time applications across web and mobile platforms without complex server setups.
**Ref:** https://medium.com/@nakrani/what-is-webrtc-6e9cc2a51222
#### SDP (Session Description Protocol)
SDP (Session Description Protocol) is a format for describing multimedia communication sessions. In WebRTC, SDP is used for:
- Negotiating Capabilities: Determines the codecs, media types, and formats that each peer supports.
- Establishing Media Session: SDP messages (Offer/Answer) are exchanged to establish parameters for audio, video, or data channels.
- ICE Candidates: ICE candidates in SDP help peers find the best route for data transmission.
### CoTURN
CoTURN is an open-source server for TURN and STUN protocols. It helps clients behind firewalls connect for voice, video, and data sharing. CoTURN relays data between the client and server, making the connection appear to come from the server. This is essential for apps needing real-time communication, like WebRTC, especially when direct peer-to-peer connections aren’t possible.
- STUN finds a device’s public IP behind a firewall. This allows direct connections between devices.
- TURN relays data when direct connections are blocked. TURN is critical for WebRTC’s reliable performance.
In WebRTC, CoTURN uses the ICE protocol for checking connectivity and setting up real-time connections, even through restrictive firewalls.
- Many enterprise firewalls only allow specific apps and websites. They block most UDP connections.
- These firewalls need authorized TURN servers, like CoTURN, for WebRTC media streams.
- CoTURN can separate business and social traffic using first and third-party authorization.
Advantages of CoTURN:
- Firewall Traversal: It allows media and data sharing in restricted networks.
- Reliability: CoTURN ensures reliable connections when direct peer-to-peer isn’t possible.
- Security: It supports encryption, helping protect data.
- Enterprise-Friendly: CoTURN works well with enterprise firewall policies, allowing controlled WebRTC connections.

## App
![image](https://github.com/user-attachments/assets/72a73bce-7787-4277-b5af-4dbd1bd6f660)
The objective is to make a call between two peers using WebRTC. One is an offerer which is sending audio and one is the sender which plays audio.
### Call Flow Outline
1. Initialize the Server: Start by running server.js, which acts as the signaling server, facilitating message exchange between peers.
2. Open Application Instances: Open two instances of the application (the exe file), one for each peer (offerer and answerer).
3. Set IDs and Start Call:
  - Set caller ID and target ID for each peer.
  - Click the call button on the offerer's side to initiate the connection.
  - Upon setting IDs, the peers will register with the signaling server.
4. Exchange SDP (Session Description Protocol):
  - Once registered, the offerer will create an SDP offer, which is sent to the signaling server.
  - The signaling server routes the offer SDP to the answerer.
  - The answerer generates an SDP answer and returns it to the offerer through the signaling server.
5. Set Local and Remote SDP:
  - Each peer sets the remote SDP received from the other peer.
  - ICE (Interactive Connectivity Establishment) candidates are exchanged for connectivity.
6. Establish Connection: Once local and remote SDPs are set and ICE gathering is completed, the peer connection is established.
