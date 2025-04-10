// --- State Management ---
import processing.serial.*;
int HAPPY = 0;
int SAD_AGITATED = 1;
int SLEEPING = 2;

Serial myPort;
boolean[] orders;
String val;
int faceState = HAPPY; // Initial state
int previousFaceState = HAPPY; // To remember state before sleeping

// ArrayLists to hold multiple tears
ArrayList<Tear> leftTears;
ArrayList<Tear> rightTears;

// --- Tear Class (remains the same) ---
class Tear {
  float x, y;
  float speed;
  float tearWidth = 8;
  float tearHeight = 12;

  Tear(float startX, float startY) {
    x = startX;
    y = startY;
    speed = random(1, 3);
  }

  void fall() {
    y += speed;
  }

  void display() {
    fill(100, 150, 255, 200);
    noStroke();
    ellipse(x, y, tearWidth, tearHeight);
  }

  boolean isOffScreen() {
    return (y > 60);
  }
} // --- End of Tear Class ---


void setup() {
  size(200, 200);
  ellipseMode(CENTER);
  orders = new boolean[]{false,false,false};

  // Initialize the ArrayLists
  leftTears = new ArrayList<Tear>();
  rightTears = new ArrayList<Tear>();
  
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
  // Initial instructions
  println("Face starts happy.");
  println("Press any key (except '0') to make it sad/agitated.");
  println("Press '0' to toggle sleep.");
}

void draw() {
  update();
  background(220); // Light gray background
  pushMatrix();
  translate(100, 100); // Center the coordinate system

  // --- Apply Agitation ONLY if Sad ---
  if (faceState == SAD_AGITATED) {
    float agitationAmount = 1.5;
    float offsetX = random(-agitationAmount, agitationAmount);
    float offsetY = random(-agitationAmount, agitationAmount);
    translate(offsetX, offsetY); // Apply the shake
  }

  // --- Draw Head (Always) ---
  fill(0, 255, 0); // Green
  noStroke();
  ellipse(0, 0, 150, 150); // Head

  // --- Draw Eyes (State Dependent) ---
  if (faceState == SLEEPING) {
    // --- Sleeping Eyes (Closed) ---
    stroke(0);      // Black lines for closed eyes
    noFill();       // Don't fill the arc
    strokeWeight(2); // Slightly thicker line
    // Draw downward arcs for closed eyes
    arc(-25, -15, 30, 15, 0, PI); // Left closed eye arc
    arc(25, -15, 30, 15, 0, PI);  // Right closed eye arc
    strokeWeight(1); // Reset stroke weight
  } else {
    // --- Open Eyes (Happy or Sad) ---
    stroke(0); fill(255); // Black outline, white fill
    ellipse(-25, -15, 30, 20); // Left eye shape
    ellipse(25, -15, 30, 20);  // Right eye shape
    // Draw Pupils
    fill(0); noStroke();    // Black fill, no outline
    ellipse(-25, -15, 10, 10); // Left pupil
    ellipse(25, -15, 10, 10);  // Right pupil
  }

  // --- Draw Mouth & Tears (State Dependent) ---
  if (faceState == HAPPY) {
    // --- Happy Mouth (Smile) ---
    fill(255, 0, 0); stroke(255, 0, 0); // Red
    arc(0, 25, 80, 50, 0, PI);         // Smile arc (bottom half)
    line(-40, 25, 40, 25);             // Connecting line
  } else if (faceState == SAD_AGITATED) {
    // --- Sad Mouth (Frown) ---
    fill(255, 0, 0); stroke(255, 0, 0); // Red
    arc(0, 25, 80, 50, PI, TWO_PI);    // Frown arc (top half)
    line(-40, 25, 40, 25);             // Connecting line

    // --- Generate, Update, Draw, and Remove Tears ---
    // Generate new tears occasionally
    if (random(1) < 0.15) {
       leftTears.add(new Tear(-25, 5));
       rightTears.add(new Tear(25, 5));
    }
    // Process Left Tears
    for (int i = leftTears.size() - 1; i >= 0; i--) {
      Tear t = leftTears.get(i);
      t.fall();
      t.display();
      if (t.isOffScreen()) { leftTears.remove(i); }
    }
    // Process Right Tears
    for (int i = rightTears.size() - 1; i >= 0; i--) {
      Tear t = rightTears.get(i);
      t.fall();
      t.display();
      if (t.isOffScreen()) { rightTears.remove(i); }
    }
  } else if (faceState == SLEEPING) {
    // --- Sleeping Mouth ---
    stroke(0); // Black line
    line(-20, 25, 20, 25); // Simple neutral horizontal line
  }

  popMatrix(); // Restore original coordinate system
} // End of draw()

void updateOrders(){
  val = readSerial();
  if(val != ""){
    orders = boolean(int(split(val, ",")));
  }
}

String readSerial(){
  if ( myPort.available() > 0) {  // If data is available,
    return myPort.readString();// read it and store it in val
  }
  return "";
}


void update() {
  updateOrders();
  if (myPort != null && orders.length == 3) {
    if (orders[0] && !orders[1] && !orders[2]) {
      faceState = SLEEPING;
      leftTears.clear();
      rightTears.clear();
      println("Going to sleep...");
    } else if (orders[1] || orders[2]) {
      faceState = SAD_AGITATED;
    } else {
      faceState = HAPPY;
    }
    // If sleeping, keys other than '0' do nothing
  }
}
