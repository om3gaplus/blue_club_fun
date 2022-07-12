setup="void setup() {"
loop="void loop() {"
terminal=input("enter the terminal you used: ")
delay=input("enter delay: ")
setup+="pinMode("+terminal+", OUTPUT);}"
loop+='''for(int i=0;i<=255;i++){
    analogWrite('''+terminal+''',i);
    delay('''+delay+''');
  }
  for(int j=255;j>=0;j--){
    analogWrite('''+terminal+''',j);
    delay('''+delay+''');    
  }
  
}'''
print(setup+loop)
