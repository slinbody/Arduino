class DigitalPin
{
	public:
		DigitalPin(int pin);
		void begin();
		void on();
		void off();
                bool status();
	private:
		int _pin;
		bool _status;
};

DigitalPin::DigitalPin(int pin)
{
	_pin = pin;
        
}
void DigitalPin::begin()
{
	pinMode(_pin, OUTPUT);
}
void DigitalPin::on()
{
	digitalWrite(_pin, HIGH);
	_status=1;
}
void DigitalPin::off()
{
	digitalWrite(_pin,LOW);
	_status=0;
}
bool DigitalPin::status()
{
	return _status;
}

DigitalPin LED[2]={12,13};


void setup()
{
	LED[0].begin();
	LED[1].begin();
}

void loop()
{
	LED[0].on();
	delay(500);
	LED[0].off();
	delay(500);
	LED[1].on();
	delay(500);
	LED[1].off();
	delay(500);

}
