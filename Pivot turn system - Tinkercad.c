#include <Servo.h>
#include <Keypad.h>

#define Password_Length 5 

char Data[Password_Length]; 
char Master[Password_Length] = "3874"; 
byte data_count = 0, master_count = 0;
bool Pass_is_good = true;
char customKey;

const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {9,8,7,6}; //Rows 0 to 3
byte colPins[numCols]= {5,4,3,2}; //Columns 0 to 3

Servo myservo;

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

//Setup the serial monitor and servo
void setup()
{
Serial.begin(9600);
myservo.attach(11);
pinMode(12,OUTPUT); //Green LED
pinMode(13,OUTPUT); //Red LED
}

//The main loop
void loop()
{
myservo.write(0);
digitalWrite(12,LOW);
digitalWrite(13,HIGH);

//Process key pad inputs
if (Pass_is_good) 
{
Serial.println("Enter Password: ");
Pass_is_good = false;
}
customKey = myKeypad.getKey();

if (customKey)
{
	Serial.println(customKey);
Data[data_count] = customKey;
data_count++;
}

if(data_count == Password_Length-1)
{
Serial.println(Data);

if(!strcmp(Data, Master))
{
Serial.println("*** Password Correct! Your encrypted key code is: ***");
digitalWrite(13,LOW);
digitalWrite(12,HIGH);
displayEncryptedKeyCode();
Pass_is_good = true;
activatelock();
}
			else
{
Serial.println("Password Incorrect!");
digitalWrite(13,HIGH);
delay(1000);
Pass_is_good = true;
}

clearData();
}
}


//Clear user entered data
void clearData()
{
//Clear variables
while(data_count !=0)
{
Data[data_count--] = 0;
}
return;
}


//Activate the lock once the correct code has been entered
void activatelock()
{
digitalWrite(12,HIGH);

//Servo control
for (int pos = 0; pos <= 90; pos += 1)
{
//In steps of 1 degree
myservo.write(pos);
delay(15);
}
delay(5000);

for (int pos = 90; pos >= 0; pos -= 1)
{
myservo.write(pos);
delay(15);
}
}

//Display the entered code 
void displayEncryptedKeyCode() 
{
//Key code encryption
char encryptedkeycode[Password_Length]; 
itoa( atoi(Data), encryptedkeycode, 16);
Serial.println( encryptedkeycode );
Serial.println("*****************************************************");

}
