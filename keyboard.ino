//BEGIN CONFIG

//The colemak keyboard layout is defined below.
//rows are outputs, columns inputs.
const int layout[8][13] = {
	{KEY_LEFT_ALT, ' ', KEY_DELETE, KEY_LEFT_SHIFT, KEY_LEFT_GUI, 0, 0, 'b', 'k', 0, 0, '/', 0},
	{0, 0, KEY_LEFT_CTRL, 0, 'z', 'x', 'c', 'v', 'm', ',', '.', KEY_ENTER, KEY_ESC},
	{0, 0, 0, 0, 0, 0, 0, 'g', 'j', ']', 0, '[', '\\'},
	{0, 0, KEY_TAB, 0, 'q', 'w', 'f', 'p', 'l', 'u', 'y', ';', 0},
	{0, 0, 0, 0, '1', '2', '3', '4', '7', '8', '9', '0', 0},
	{0, 0, 0, 0, '`', 0, 0, '5', '6', '=', 0, '-', KEY_BACKSPACE},
	{0, 0, 0, 0, 'a', 'r', 's', 't', 'n', 'e', 'i', 'o', 0},
	{0, 0, 0, 0, 0, 0, 0, 'd', 'h', 0, 0, '\'', 0}};

//mouse button coordinates (evens are rows, odds columns)
const int mbcs[6] = {0, 0, 0, 0, 0, 0};

//pin definitions
const int rows[8] = {13, 14, 15, 16, 17, 18, 19, 20};
const int cols[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

//END CONFIG

/*
   How the keyboard works:
   A keyboard, in its most basic form is a matrix of buttons.
   That means that the buttons form a grid.
   Each column has a wire that connects to one end of its switch.
   Each row has a wire that connects to the other end of its switch.
   To see the state of the switches, a microcontroller will put a current
   through the first row. It will then read the first column and record the
   state. Then the second column and so on. Then, it will repeat with the
http://dread.life/keyboard.ino   other rows.
   It will compare the state of the newly read switches with what was read
   before. 
   It will send changes to the computer.

   Electrical signals are inverted. HIGH is off; LOW is on.
   This is because arduino only has pullup resistors, not pulldown.

   How the mouse works:
   It's just two potentiometers, and it sends the difference in position to
   the computer.
 */

//active keys on the keyboard
char fresh[8][13] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

unsigned long lastpressed[8][13] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

//last state of the keyboard
char last[8][13];

//no setup
void setup() {
	int r, c;
	Serial.begin(9600);
	Keyboard.begin();
	for (r = 0; r < 8; r++) {
		pinMode(rows[r], OUTPUT);
		digitalWrite(rows[r], HIGH);
	}
	for (c = 0; c < 13; c++) {
		pinMode(cols[c], INPUT_PULLUP);
	}
}

void loop() {
	int r, c;
	for (r = 0; r < 8; r++) {
		digitalWrite(rows[r], LOW);
		for (c = 0; c < 13; c++) {
			if (millis() - lastpressed[r][c] > 50) { //for debouncing
				//Transfer the new matrix to the old matrix for comparison
				last[r][c] = fresh[r][c];
				//get state
				int dr = !digitalRead(cols[c]);
				fresh[r][c] = dr;
				//send state
				if (last[r][c] != fresh[r][c]) {
					lastpressed[r][c] = millis();
					/* For debugging or finding keys:
					   Serial.print("K: (r:");
					   Serial.print(r);
					   Serial.print(", c:");
					   Serial.print(c);
					   Serial.print(") ");
					   Serial.print(dr);
					   Serial.println(layout[r][c]);
					 */
					if (dr) {
						Keyboard.press(layout[r][c]);
					} else {
						Keyboard.release(layout[r][c]);
					}
				}
			}
		}
		digitalWrite(rows[r], HIGH);
	}
}
