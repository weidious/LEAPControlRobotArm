#include <iostream>
#include <string.h>
#include "Leap.h"
#include <unistd.h>
#include<ctime>
#include <sys/time.h>

using namespace Leap;
using namespace std;
const string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};
class SampleListener : public Listener {
        public:
            virtual void onConnect(const Controller&);
            virtual void onFrame(const Controller&);
            virtual void onInit(const Controller&);
    

};
void SampleListener::onInit(const Controller& controller) {
      cout << "Initialized" << endl;
}
void SampleListener::onConnect(const Controller& controller) {
            cout << "Connected" << endl;
        controller.enableGesture(Gesture::TYPE_CIRCLE);
        controller.enableGesture(Gesture::TYPE_KEY_TAP);
        controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
        controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onFrame(const Controller& controller) {
 //sleep(1);
  timeval tim;
  const Frame frame = controller.frame();
   gettimeofday(&tim,NULL);
    double t1 = tim.tv_usec;
        cout<<"Time"<<t1<<endl;
      cout << "Frame available : " << frame.id()
            << ", timestamp: " << frame.timestamp()
            << ", hands: " << frame.hands().count()
            << ", extended fingers: " << frame.fingers().extended().count()
            << ", number of gestures get: " << frame.gestures().count() << endl;
   HandList hands = frame.hands();
     for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
     // Get hands in sequence 
         const Hand hand = *hl;//take over the iterator's parameter
      string handType = hand.isLeft() ? "Left hand" : "Right hand";
	     cout << handType << ", id: " << hand.id()
              << ", palm position: " << hand.palmPosition() << endl;
     // Get the hand's normal vector and direction
    	 const Vector normal = hand.palmNormal();
    	 const Vector direction = hand.direction();
			cout <<"pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
              << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
              << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << endl;

    // Get the Arm bone
    	Arm arm = hand.arm();
    		cout<<  "Arm direction: " << arm.direction()
              << " wrist position: " << arm.wristPosition()
              << " elbow position: " << arm.elbowPosition() << endl;

    // Get fingers
    const FingerList fingers = hand.fingers();
    for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
      const Finger finger = *fl;
       cout << string(4, ' ') <<  fingerNames[finger.type()]
                << " finger, id: " << finger.id()
                << ", length: " << finger.length()
                << "mm, width: " << finger.width() <<  endl;

      // Get finger bones
      for (int b = 0; b < 4; ++b) {
        Bone::Type boneType = static_cast<Bone::Type>(b);
        Bone bone = finger.bone(boneType);
         cout << string(6, ' ') <<  boneNames[boneType]
                  << " bone, start: " << bone.prevJoint()
                  << ", end: " << bone.nextJoint()
                  << ", direction: " << bone.direction() <<  endl;
      }
    }

   }
const GestureList gestures = frame.gestures();
  for (int g = 0; g < gestures.count(); ++g) {
    Gesture gesture = gestures[g];

    switch (gesture.type()) {
      case Gesture::TYPE_CIRCLE:
      {
        CircleGesture circle = gesture;
         string clockwiseness;

        if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
          clockwiseness = "clockwise";
        } else {
          clockwiseness = "counterclockwise";
        }

        // Calculate angle swept since last frame
        float sweptAngle = 0;
        if (circle.state() != Gesture::STATE_START) {
          CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
          sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
        }
         cout << string(2, ' ')
                  << "Circle id: " << gesture.id()
                  << ", state: " << stateNames[gesture.state()]
                  << ", progress: " << circle.progress()
                  << ", radius: " << circle.radius()
                  << ", angle " << sweptAngle * RAD_TO_DEG
                  <<  ", " << clockwiseness <<  endl;
        break;
      }
      case Gesture::TYPE_SWIPE:
      {
        SwipeGesture swipe = gesture;
         cout << string(2, ' ')
          << "Swipe id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", direction: " << swipe.direction()
          << ", speed: " << swipe.speed() <<  endl;
        break;
      }
      case Gesture::TYPE_KEY_TAP:
      {
        KeyTapGesture tap = gesture;
         cout << string(2, ' ')
          << "Key Tap id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", position: " << tap.position()
          << ", direction: " << tap.direction()<<  endl;
        break;
      }
      case Gesture::TYPE_SCREEN_TAP:
      {
        ScreenTapGesture screentap = gesture;
         cout << string(2, ' ')
          << "Screen Tap id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", position: " << screentap.position()
          << ", direction: " << screentap.direction()<<  endl;
        break;
      }
      default:
         cout << string(2, ' ')  << "Unknown gesture type." <<  endl;
        break;
    }
  }

  
}
int main(int argc, char** argv) {
    SampleListener listener;
    Controller controller;
        controller.addListener(listener);
        
            cout << "Press Enter to quit..." << endl;
            cin.get();
        
        controller.removeListener(listener);

                    return 0;
}
