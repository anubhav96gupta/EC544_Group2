Task: Create a swarm with the Arduino-XBee or Photons. The swarm will react to an “infection” triggered by a button push.

Basic Elements:
1. There are three LEDs. The Blue indicates	a leader; the Green indicates not infected; and the	Red	indicates infected.
2. Pressing	the	button with	either start an infection or clear an infection depending on being a leader or a non-leader.
3. One leader is elected on	startup per	connected network.
4. A new leader is elected if an existing leader disappears or is turned off.
5. While infected, each device will send out infection messages to other connected devices with a fixed period of 2 seconds.
6. A clear infection message has priority over infection message.

Leader Specific:
1. The leader is immune to infections and cannot infect itself.	
2. Pressing the button will cause a leader to send a clear infection message to other devices. This should happen only once per button press (not continuous).
3. It is possible to have reinfections if not all devices are immediately reachable.

Non-Leade Specific:
1. Pressing the button causes a non-leader to infect itself.
2. Upon receiving a clear infection message, a non-leader will return to the not infected state and propagate the clear infection message to its neighbors.
3. Infections are persistent; repeat infections can occur if any of the non-leaders in the network are not cleared.
4. A non-leader is immune to infection for 3 seconds following a clear infection message to prevent immediate re-infection.


Video - https://www.youtube.com/watch?v=eFDwL-RY_rk&feature=youtu.be
