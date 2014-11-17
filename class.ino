class A
{
  public:
   int x;
   void f() { x=1; }
};

class B : public A
{
  public:
    int y;
    void f() { x=2; }
};


A *a;
B *b;
const int TEST_PIN = 13;

void setup()
{
   a=new A(); 
   b=new B();
   pinMode(TEST_PIN,OUTPUT);
}

void loop()
{
   a->f();
   b->f();
   digitalWrite(TEST_PIN,(a->x == b->x) ? HIGH : LOW);
}
