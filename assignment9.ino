#include <Smartcar.h>
#include <SoftwareSerial.h>

Odometer encoderLeft(190), encoderRight(190); //Βάλτε τους δικούς σας παλμούς ανά μέτρο
Gyroscope gyro(5); //Βάλτε την κατάλληλη τιμή σύμφωνα με το γυροσκόπιό σας
Car folkracer;
SR04 sonarLeft, sonarRight, sonarFront; //dilwse tis metavlites sonarLeft, sonarRight kai sonarFront pou antiproswpevoun tous iperixous
SoftwareSerial bluetooth(6, 7); //συνδέστε το bluetooth ως εξής: Το RX του Bluetooth στο pin 6 και το ΤΧ του bluetooth στο pin 7 (VCC -> 5V, GND -> GND)
int degrees = 20 ;
int motorSpeed = 50;

/* Χρησιμοποιήστε τα σωστά pins!!!! */
const int SONAR_LEFT_TRIGGER = 4; //trigger του αριστερού υπέρηχου
const int SONAR_LEFT_ECHO = 5; //echo του αριστερού υπέρηχου
const int SONAR_RIGHT_TRIGGER = A0; //trigger του δεξιού υπέρηχου
const int SONAR_RIGHT_ECHO = A1; //echo του δεξιού υπέρηχου
const int SONAR_FRONT_TRIGGER = A2; //trigger του μπροστινού υπέρηχου
const int SONAR_FRONT_ECHO = A3; //echo του μπροστινού υπέρηχου

void setup() {
  bluetooth.begin(9600); //ξεκινήστε τη σειριακή σύνδεση
  sonarLeft.attach(SONAR_LEFT_TRIGGER, SONAR_LEFT_ECHO); // αρχικοποίησε τον υπέρηχο στα κατάλληλα pins
  sonarRight.attach(SONAR_RIGHT_TRIGGER, SONAR_RIGHT_ECHO);
  sonarFront.attach(SONAR_FRONT_TRIGGER, SONAR_FRONT_ECHO);
  gyro.attach(); //αρχικοποιεί το γυροσκόπειο
  encoderLeft.attach(3); //αρχικοποιεί το encoder
  encoderRight.attach(2);
  encoderLeft.begin();//ξεκινάει τις μετρήσεις στον encoder
  encoderRight.begin();
  folkracer.begin(encoderLeft, encoderRight, gyro); //ξεκινάει το αυτοκίνητο χρησιμοποιώντας τα encoders και το γυροσκόπειο
 // folkracer.enableCruiseControl(); //ξεκινάει τον έλεγχο της ταχύτητας του αυτοκινήτου
  gyro.begin(); //ξεκινάει τις μετρήσεις στο γυροσκόπειο
  /* Εάν θέλετε να διαβάσετε απλά τις αποστάσεις, δίχως να κινείται το αυτοκινητάκι, βάλτε ταχύτητα 0 στην παρακάτω γραμμή */
  folkracer.setSpeed(motorSpeed); //θέτει την ταχύτητα στο αυτοκινητάκι στα 0.2 μέτρα ανά δευτερόλεπτο (εάν είναι πολύ αργό, αυξήστε λίγο την ταχύτητα)
  folkracer.setAngle(0); //Το αυτοκινητάκι πηγαίνει ευθεία
}

void loop() {
  folkracer.updateMotors(); //διατήρησε την ταχύτητα του αυτοκινήτου σταθερή
  int frontDistance = sonarFront.getMedianDistance(6); //μέτρησε την απόσταση (φιλτράρισε 6 μετρήσεις) και αποθήκευσέ την στη μεταβλητή frontDistance
  int leftDistance = sonarLeft.getMedianDistance(6);
  int rightDistance = sonarRight.getMedianDistance(6);
  bluetooth.print(frontDistance); //εκτύπωσε τις αποστάσεις με την εξής μορφή: μπροστινήΑπόσταση,αριστεράΑπόσταση,δεξιάΑπόσταση
  bluetooth.print(",");
  bluetooth.print(leftDistance);
  bluetooth.print(",");
  bluetooth.println(rightDistance);
  //TO-DO για το σχολείο: Κινήστε το αυτοκινητάκι στην πίστα και ΑΣΥΡΜΑΤΑ δείτε τις αποστάσεις στον υπολογιστή, μέσω bluetooth (δεν χρειάζεται επιπλέον κώδικας)
  //TO-DO για το σπίτι: Κάντε το αυτοκινητάκι να προσπαθεί να μείνει στη μέση των δύο τοίχων. Για αρχή, δοκιμάστε αυτή τη λογική: Εάν απέχει από τον δεξιά τοιχο περισσότερο
  //από ότι τον αριστερό, στρίψτε δεξιά, αλλιώς αριστερά. Το πόσο θα στρίψετε αριστερά, με το folkracer.setAngle θα το βρείτε πειραματικά. Αρχίστε τις δοκιμές με το setAngle(50).
  //Εάν βρίσκει εμπόδιο μπροστά σε κοντινή απόσταση, ας στρίβει προς τα δεξιά κατά 75 μοίρες, με το setAngle.
  //Στο σχολείο, να διαβάζετε (στο serial monitor του Arduino IDE) τις αποστάσεις που ανιχνεύει το αυτοκινητάκι ασύρματα μέσω bluetooth, έτσι ώστε να καταλάβετε τι "διαβάζει"
  //το αυτοκινητάκι όταν κινείται στην πίστα!
  if (leftDistance == 0 && rightDistance == 0) { //c3
    folkracer.setAngle(0);
    bluetooth.println("pigainei efthia c3");
  }
  if (rightDistance < 30 && rightDistance > 0 && leftDistance < 30 && leftDistance > 0) { //d4
    folkracer.setAngle(0);
    bluetooth.println("pigainei efthia c4");
  }
  if (rightDistance < 30 && rightDistance > 0 && leftDistance == 0) { //d3
   // folkracer.setAngle(-80);
    bluetooth.println("strivei aristera d3");
   //rotateOnSpot(-degrees);
  }
  if (rightDistance > 30 && leftDistance == 0) { //e3
    folkracer.setAngle(0);
    bluetooth.println("pigainei efthia e3");
  }
  if (leftDistance < 30 && leftDistance > 0 && rightDistance == 0) { //c4
    //folkracer.setAngle(80);
    bluetooth.println("strivei dexia c4");
   // rotateOnSpot(degrees);  
  }
  if (rightDistance > 30 && leftDistance < 30 && leftDistance > 0) { //e4 
    //folkracer.setAngle(80);
    bluetooth.println("strivei dexia e4");
   // rotateOnSpot(degrees);
  }
  if (rightDistance == 0 && leftDistance > 30) { //c5 
    folkracer.setAngle(0);
    bluetooth.println("pigainei efthia c5");
  } 
  if (rightDistance < 30 && rightDistance > 0 && leftDistance > 30) { //d5
   // folkracer.setAngle(-80);
    bluetooth.println("strivei aristera d5");
   // rotateOnSpot(-degrees);
  }
  if (rightDistance > 30 && leftDistance > 30) { //e5
    folkracer.setAngle(0);
    bluetooth.println("pigainei efthia e5");
  }
  if (frontDistance < 20 && frontDistance > 0) { //g6
   // folkracer.setAngle(80);
    bluetooth.println("strivei dexia g6");
    //rotateOnSpot(degrees);
  }
}
/*void rotateOnSpot(int targetDegrees){
  if (!targetDegrees) return; //if the target degrees is 0, don't bother doing anything
  gyro.begin(); //initiate the measurement
  // Let's set opposite speed on each side of the car, so it rotates on spot 
  if (targetDegrees > 0){ //positive value means we should rotate clockwise
    folkracer.setMotorSpeed(motorSpeed, -motorSpeed); // left motors spin forward, right motors spin backward
  }else{ //rotate counter clockwise
    folkracer.setMotorSpeed(-motorSpeed, motorSpeed); // left motors spin backward, right motors spin forward
  }
  while (abs(gyro.getAngularDisplacement()) <= abs(targetDegrees)){ //while we have not reached the desired degrees
    gyro.update(); //update the gyro readings
  }
 // folkracer.stop(); //we have reached the target, so stop the car
}*/
