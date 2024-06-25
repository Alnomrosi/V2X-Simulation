//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "veins/modules/application/traci/MyVeinsApp.h"

using namespace veins;

Define_Module(veins::MyVeinsApp);

void MyVeinsApp::initialize(int stage)
{
    static int ID_Counter = 0 ;
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        // Initializing members and pointers of your application goes here
        // par("appName").stringValue()
        EV << "Initializing " << "CAM Application" << std::endl;
        EV << "Init Vehicle " << getId() << endl ;
      //  EV << "Init Veins app Node " << myId << endl ;
    }
    else if (stage == 1)
    {
      // Initializing members that require initialized other modules goes here
    }
}


void MyVeinsApp::finish()
{
    DemoBaseApplLayer::finish();
    // statistics recording goes here
}
int Global_SenderId = 0 ;
void MyVeinsApp::sendBSM() {
    // Counter for the ID Change//
     static int ID_Counter = 0 ;

    // Create a new DemoSafetyMessage
    DemoSafetyMessage* bsm = new DemoSafetyMessage("CAM",0);

    // Populate BSM fields
    bsm->setSenderPos(curPosition);
    bsm->setSenderSpeed(curSpeed);
    bsm->setPsid(-1);
    bsm->setChannelNumber(static_cast<int>(Channel::cch));
    bsm->addBitLength(beaconLengthBits);
    float Speed = traciVehicle->getSpeed() ;

    // Save Sender ID
    if (ID_Counter >= 3)
    {
       Global_SenderId = getId() + (rand() % 100);
    }
    ID_Counter += 1;
    // Log the Sending BSM information
    //EV << "Sending.. cam:" << endl;
    EV << "Sender Id: " << Global_SenderId << endl;
    EV << "Position " << curPosition ;
    EV << " Speed " << Speed << " kph" << endl ;

  // Send the message down to the lower layers for transmission
    sendDown(bsm);
}


void MyVeinsApp::onBSM(DemoSafetyMessage* bsm)
{
    // Your application has received a beacon message from another car or RSU
    // code for handling the message goes here

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Extract information from the received BSM message
    Coord senderPos = bsm->getSenderPos(); // Assuming Coord is used for position
    Coord senderSpeed = bsm->getSenderSpeed();
    int psid = bsm->getPsid();
    int channelNumber = bsm->getChannelNumber();
    int bitLength = bsm->getBitLength();
    int userPriority = bsm->getUserPriority();


    //Sender position and speed
    // EV << " - Sender Position: (" << senderPos.x << ", " << senderPos.y << ", " << senderPos.z << ")\n";
   // EV << " - Sender Speed: " << senderSpeed << " m/s\n";
    ///////////////////////////////////////////////////////////////////////////////////////////////////


}

void MyVeinsApp::onWSM(BaseFrame1609_4* wsm)
{
    // Your application has received a data message from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples
}

void MyVeinsApp::onWSA(DemoServiceAdvertisment* wsa)
{
    // Your application has received a service advertisement from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples
}

void MyVeinsApp::handleSelfMsg(cMessage* msg)
{
    DemoBaseApplLayer::handleSelfMsg(msg);

    switch (msg->getKind()) {
        case SEND_BEACON_EVT: { // Event for sending BSM
            //EV << "SEND_BEACON_EVT" <<endl ;
            sendBSM(); // Call the function to create and send a BSM
            scheduleAt(simTime() + beaconInterval, sendBeaconEvt);
            break;
        }
        default:
            EV << "Unknown self message received\n";
            break;
    }

}

void MyVeinsApp::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);
    // the vehicle has moved. Code that reacts to new positions goes here.
    // member variables such as currentPosition and currentSpeed are updated in the parent class
}
