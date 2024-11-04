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
The objective is to make a call between two peers using WebRTC. One is an offerer which is sending audio and one is the sender which plays audio. First, we need to run the server.js file. Second, we need to open two windows of the exe file. Then set the caller and calling IDs for the offerer and the caller ID for the offerer. Then click on the call button for the offerer. When you set IDs, peers will be registered on the signaling server. The signaling server will show, which peer is registered. Then, a SDP will generate as an answerer. After clicking on the call button, the type of the registered peer will be changed and it changes the sdp type before sending to the server, the server will show that it recieves some messages from the peer and it will be informed with the target id. it sends the local sdp of offerer to the target peer id. Then, when the target receives, it will generate and answer which is its new local sdp, then send it to signaling server. Also, it sets the remote sdp. signaling server will route the answer to the caller. The caller sets its remote sdp. The ICE gathering started and when both local and remote sdps are set, the peer connection will be connected.
