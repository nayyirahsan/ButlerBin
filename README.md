Butler Bin



Contributors:

Nayyir Ahsan. I was the one who created the idea of Butler Bin. Moreover, I was in charge of the actual trash can function (automatic opening lid, LED status screen, leak detection, and trash level detection). 
I also designed the trashcan using cardboard so all group members could work with the same product. Lastly, I was the one who adjusted and integrated every member’s contribution into one cohesive product.

Mimi Chay. She was in charge of the structural design (mounts), the motor configurations, and the movement. The Arduino Mounts were found online.

Gabriel Park. He was in charge of the navigation system that told the robot where to go and where to come back to. He based his code on the following link: https://www.youtube.com/watch?v=0Yvd_k0hbVs

Jai Sharma. He was in charge of developing an app that the user could use to tell the robot to go to them. 




Purpose:

The project is designed to help people with mobility issues and enhance elderly home cleanliness. The individual using this product would press a button on the remote or app and then the robot would locate and go to them. 
Then the user can wave their hand over the trash can lid for it to open and dispose of their trash. Afterward, the robot would turn around to go back to where it came from. This product is convenient because there is not always a trash can next to everyone, 
resulting in people leaving their trash on tables or the floor. By having a product that will come and pick up your trash with just a click of a button, people with limited mobility can ensure a clean space without having to strain themselves. 
This project was a part of the MIT Beaverworks Summer Institute so we only had $120 and two weeks to fully design and create this product.



How:

Butler Bin is an automated trash receiver. It has a remote that when clicked, sends the trash can to the user. Then, the user can engage with the additional functionality of the automatic opening lid (PIR sensor detects movement, 
triggering a servo motor to lift the lid), leak detection (water level sensor detects spills, prompting a water droplet on the screen), trash level detection (ultrasonic sensor detects when the can is full, prompting a frowny face on the screen), 
and LED status screen. Finally, the robot then returns to its original location. It works by using 4 ESPs (two on the walls, one on the trash can, and one on the remote control) to triangulate the coordinates of the trash can and user. 
Afterward, the robot leverages encoders to accurately face the correct direction and travel the right amount of distance. Finally, the robot reverses its actions to return to its original spot.
