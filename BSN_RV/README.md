# Adaptive Runtime Verification of the BSN

## Scenarios
The following scenarios can be invoked by writing the corresponding commands into the Serial interface of the Monitor Arduino:
* 'add': performs an adaptation in the verifier to add the glucometer
* 'remove': performs an adaptation in the verifier to remove the thermometer
* 'event': performs an adaptation in the verifier to change the event corresponding to the Request from the BodyHub to the Scheduler
* 'separate': performs an adaptation in the verifier to switch to the Response pattern only (only one response)

### Detailed Description

#### Scenario 1: Adding a Sensor node
Consider the example our paper. At runtime, the patient is diagnosed to be diabetic and the BSN should be equipped with a glucose sensor. This changes the requirement because now there are more sensors than previously. Now, *all three* sensor nodes have to send data to the BodyHub.
In the property, this reflects as:

     Within each scheduler cycle, all *three* sensor nodes shall be executed in turn within 2s.
     
thus acknowledging that the system now consists of three sensor nodes. We utilize the PAP Adding a Response to the Chain to represent the change.

#### Scenario 2: Updating the time-bound  
Afterward, the stakeholders notice network congestions and update the time-bound within which the sensors are expected to send their data from 2s to 3s.


Following the PAP Updating the Time Guard, this change reflects in the property:


     Within each scheduler cycle, all three sensor nodes shall be executed in turn within 3s.

In the observer, the guard conditions are updated accordingly.


#### Scenario 3: Removing a sensor node
At some later point, the thermometer sensor fails. The stakeholders decide that until a new sensor can be obtained, the system is to continue its operation without the thermometer. Therefore, they change the requirement to represent that two sensors are used in the BSN.

For this change, the PAP Removing a Response from the Chain can be used.
This reflects in the property in the following:
     Within each scheduler cycle, all two sensor nodes shall be executed in turn within 3.


Removing a response from the chain can be performed following the corresponding PAP.
This PAP removes a state from the automaton. Therefore, we discussed in our paper which of the remaining states should be the current state, if the current state was to be removed.
In this case, if *waiting2* is the current state, the PAP declares *waiting3*
as the current state. In this demonstration, *waiting2* symbolizes the following information: In *waiting2*, a scheduler cycle has started but not yet elapsed, the BodyHub has requested data, and the pulse sensor has sent data. In the adapted observer, this information is contained in state *waiting3*. Therefore, *waiting3* is selected as the current state if *waiting2* was the current state previously.


#### Scenario 4: Letting the scheduler request data
At some point, the developers decide that since the scheduler knows the available network, the scheduler should request the sensors to send data to the BodyHub, instead of the BodyHub itself.
Therefore, the requirement is changed slightly and the change manifests in the observer by changing the *request* label, such that it refers to the scheduler requesting data.

Obviously, any unanswered requests that the BodyHub sent out should still be answered by the sensors sending data. Therefore, we argue that an adaptation in this scenario is more suitable than a (re-)deployment of a new observer, which would neglect unanswered requests.

#### Scenario 5: Ignoring the order of the sensors
Finally, the order of the sensors sending data to the BodyHub is deemed to be irrelevant. Therefore, the requirements manager decides to change the requirement in that regard.
Accordingly, PAP Separating the Response Chain into multiple Response Properties is applied, which reflects in the property as an adaptation towards a Response pattern with only one response, instead of a chain of multiple responses. Multiple such response properties are formulated, one for each sensor node, such as:

      Within each scheduler cycle, the pulse sensor node shall be executed within 3s.

The property of the glucose sensor can be specified in the same manner.

After exercising the PAP, the resulting observers are obtained. Since this PAP, again, removes states from the observer, a consideration of the current state is needed. Let us consider the observer for the pulse sensor: Exercised on the example, the PAP declares that if *waiting3* is the current state, *open* should be set as the current state. This matches the observation that *waiting3* symbolizes the following information: In *waiting3*, a scheduler cycle has started but not yet elapsed, the BodyHub has requested data, and the pulse sensor has sent data. In the adapted observer, this information is contained in state *open*.

## Setup
We use Arduino Mega for the sensors (three), an Arduino Mega for the BodyHub, an Arduino Mega for the Scheduler, and one for the Monitor (runtime verifier).
The following pins should be connected:

* Pulse Sensor Pin 51 with BodyHub Pin 50
* Pulse Sensor Pin 53 with BodyHub Pin A0 and Monitor Pin A3
* Pulse Sensor Pin 10 with BodyHub Pin 44 and scheduler Pin 12
* Thermometer Pin 51 with BodyHub Pin 51
* Thermometer pin 53 with BodyHub Pin A2 and monitor Pin A4
* Thermometer Pin 10 with BodyHub Pin 46 and scheduler Pin 11
* Glucometer Pin 51 with BodyHub Pin 52
* Glucometer Pin 53 with BodyHub Pin A4 and Monitor Pin A5
* Glucometer Pin 10 with BodyHub Pin 48 and Scheduler Pin 10
* Scheduler Pin A5 with BodyHub Pin A6 and Monitor Pin 13
* Scheduler Pin 13 with BodyHub Pin A8 and Monitor Pin A0
* BodyHub Pin A15 with Monitor Pin A2 and Scheduler Pin A0
* Scheduler Pin 8 with Monitor Pin A1
