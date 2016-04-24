#include <Smartcar.h>
#include <SoftwareSerial.h>

Odometer encoderLeft(190), encoderRight(190); //Βάλτε τους δικούς σας παλμούς ανά μέτρο
Gyroscope gyro(13); //Βάλτε την κατάλληλη τιμή σύμφωνα με το γυροσκόπιό σας
Car folkracer;
SR04 sonarLeft, sonarRight, sonarFront; //dilwse tis metavlites sonarLeft, sonarRight kai sonarFront pou antiproswpevoun tous iperixous
SoftwareSerial bluetooth(6, 7); //συνδέστε το bluetooth ως εξής: Το RX του Bluetooth στο pin 6 και το ΤΧ του bluetooth στο pin 7 (VCC -> 5V, GND -> GND)

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
  folkracer.enableCruiseControl(); //ξεκινάει τον έλεγχο της ταχύτητας του αυτοκινήτου
  gyro.begin(); //ξεκινάει τις μετρήσεις στο γυροσκόπειο
  /* Εάν θέλετε να διαβάσετε απλά τις αποστάσεις, δίχως να κινείται το αυτοκινητάκι, βάλτε ταχύτητα 0 στην παρακάτω γραμμή */
  folkracer.setSpeed(0); //θέτει την ταχύτητα στο αυτοκινητάκι στα 0.2 μέτρα ανά δευτερόλεπτο (εάν είναι πολύ αργό, αυξήστε λίγο την ταχύτητα)
  folkracer.setAngle(0); //Το αυτοκινητάκι πηγαίνει ευθεία
}

void loop() {
  folkracer.updateMotors(); //διατήρησε την ταχύτητα του αυτοκινήτου σταθερή
  int frontDistance = sonarFront.getMedianDistance(3); //μέτρησε την απόσταση (φιλτράρισε 3 μετρήσεις) και αποθήκευσέ την στη μεταβλητή frontDistance
  int leftDistance = sonarLeft.getMedianDistance(3);
  int rightDistance = sonarRight.getMedianDistance(3);
  bluetooth.print(frontDistance); //εκτύπωσε τις αποστάσεις με την εξής μορφή: μπροστινήΑπόσταση,αριστεράΑπόσταση,δεξιάΑπόσταση
  bluetooth.print(",");
  bluetooth.print(leftDistance);
  bluetooth.print(",");
  bluetooth.println(rightDistance);

  if (rightDistance < 30 && rightDistance > 0) {//Αν η δεξιά απόσταση είναι < 30 και > 0
    folkracer.setAngle(-50);//Στρήψε αριστερά
    bluetooth.println("strivei aristera");
  }
  if (leftDistance < 30 && leftDistance > 0) {//Αν η αριστερή απόσταση είναι < 30 και > 0
    folkracer.setAngle(50);//Στρήψε δεξία
    bluetooth.println("strivei deksia");
  }
  if (frontDistance < 30 && frontDistance > 0) { //Αν η μπροστινή απόσταση είναι < 30 και > 0
    folkracer.setAngle(75);//Στρήψε δεξια
    bluetooth.println("strivei deksia");
  }
  if (rightDistance > 30 && leftDistance > 30) {//Αν η δεξιά απόσταση είναι > 30 και η αριστερή απόσταση > 30
    folkracer.setAngle(0);//Προχόρα ευθία
    bluetooth.println("proxoraei eutia");
  }
}

