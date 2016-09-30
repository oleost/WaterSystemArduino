/*
 Automatic water with solenoid valves.
 This goes to one plant at the time.
 So it works like this: Reads a plant moisture, if its below StartwateringPlant moisture it will set a flag that this plant is dry, and then it will water for the WateringtimePlant period.
 Then it stops watering and goes on to the next plant i line an does the same, when its finished with the last plant the program waits globalWaittime before going further.
 This time as long as one plant has started watering, it will keep watering untill StopwateringPlant is reached.

 When adding and removing plants one need to edit: numberOfPlants, StopwateringPlant, StopwateringPlant, SensorpinPlant, SolenoidoutputPlant and WateringtimePlant
 
 Also it will print the value of the soil moisture, and if its watering is turned on on serial 9600 baud. 0 means its not watering and 1 is that is watering.
*/
 
/* Define Plants, this list is just to remember yourself what is what.
plant0 is an Oregano
plant1 is an Basilikum
plant2 is an Sukkerert
plant3 is an Gressløk
plant4 is an Uteblomst
*/
 
//Define warning LED
#define led 13
 
//Define how many measurement one takes average of, and wait time between each sample
#define averageMeasuringsensor 5 // How many samples
#define averageWaittimesensor 100 // Wait time between each sample
#define globalWaittime 10000 // Wait time after checked all plants and watered every plant.

//Plants setup
#define numberOfPlants 5 // Enter how many plants you have

int StartwateringPlant[numberOfPlants] = {400, 400, 400, 400, 400}; // Add a new value for each plant, this is when it starts watering
int StopwateringPlant[numberOfPlants] = {450, 450, 450, 450, 450}; // Add a new value for each plant, this is when it stop watering.
int SensorpinPlant[numberOfPlants] = {A0, A1, A2, A3, A4}; // Analoge pin, Add a new value for each plant
int SolenoidoutputPlant[numberOfPlants] = {2, 3, 4, 5, 6}; // Digital pin, Add a new value for each plant.
int WateringtimePlant[numberOfPlants] = {2000, 2000, 2000, 2000, 2000}; // Divide by 1000 and get seconds. Add a new value for each plant.

//Setup ends here
 
//the value readed from each moisture sensor
int moisturePlant[numberOfPlants] = {0}; // Value to store average reading in
 
//the sum of the 30 times sampling
long int moistureSumPlant[numberOfPlants] = {0}; // This variable is used to store the sum of all sampled value
 
boolean keepWateringPlant[numberOfPlants] = {false};

int howmanyPlants = numberOfPlants - 1;
 
void setup() {
  Serial.begin(9600);
  for(int x = 0; x <= howmanyPlants; x++)
    {
      pinMode(SolenoidoutputPlant[x], OUTPUT);  // Makes the outputs to the solenoids
    }
 
  pinMode(led, OUTPUT);
}
 
void loop() {
  for(int PlanteLoop = 0; PlanteLoop <= howmanyPlants; PlanteLoop++)
  {
    moistureSamplingPlant(PlanteLoop);
    if (moisturePlant[PlanteLoop] < StartwateringPlant[PlanteLoop]){
      keepWateringPlant[PlanteLoop] = true; // Turn on watering plant
    }
    if (moisturePlant[PlanteLoop] > StopwateringPlant[PlanteLoop]) {
      keepWateringPlant[PlanteLoop] = false; // Turn off watering on plant
    }
    if (keepWateringPlant[PlanteLoop] == true) {
      digitalWrite(SolenoidoutputPlant[PlanteLoop], HIGH); // Open valve
      delay(WateringtimePlant[PlanteLoop]); // How long it will water
      digitalWrite(SolenoidoutputPlant[PlanteLoop], LOW);
    }
  }
  delay(globalWaittime);
}
 
void moistureSamplingPlant(int PlantNr) //Read value of plant moisture
{
  for(int i = 0; i < averageMeasuringsensor; i++)// sampling predefined.
  {
    moistureSumPlant[PlantNr] = moistureSumPlant[PlantNr] + analogRead(SensorpinPlant[PlantNr]);
    delay(averageWaittimesensor);
  }
  moisturePlant[PlantNr] = moistureSumPlant[PlantNr] / averageMeasuringsensor; //Divide to get correct reading
  Serial.print("Plant ");
  Serial.print(PlantNr);
  Serial.print(" of ");
  Serial.print(howmanyPlants);
  Serial.print(" have an moisture of " );
  Serial.print(moisturePlant[PlantNr]);
  Serial.print(". Plant watering status is ");
  Serial.print(keepWateringPlant[PlantNr]);
  Serial.println();
  moistureSumPlant[PlantNr] = 0; // Reset counting variable
}
