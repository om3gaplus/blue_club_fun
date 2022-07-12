from pyMorseTranslator import translator
encoder = translator.Encoder()
decoder = translator.Decoder()
txt=input("input yoour text: ")
morse=(encoder.encode(txt).morse)
setup="void setup() {"
loop="void loop() {"
terminal=input("enter the terminal you used: ")
setup+="pinMode("+terminal+", OUTPUT);}"
for i in morse:
    if i=='.':
        loop+="digitalWrite("+terminal+",HIGH);delay(200);digitalWrite("+terminal+",LOW);delay(200);"
    if i=="-":
        loop+="digitalWrite("+terminal+",LOW);delay(200);digitalWrite("+terminal+",LOW);delay(100);"
    if i==" ":
        loop+="digitalWrite("+terminal+",LOW);delay(200);digitalWrite("+terminal+",LOW);delay(300);"
    if i==" ":
        loop+="digitalWrite("+terminal+",LOW);delay(200);digitalWrite("+terminal+",LOW);delay(800);"
loop+="}"
print (setup+loop)
