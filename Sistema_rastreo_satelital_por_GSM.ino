#include <TinyGPS.h>
TinyGPS gps;

char numero_cell[]="985819817";//meter numero de telefono
int estado=13;
int led =12;
int i=0;
int j=0;

float lati,longi=0;
char DAT;
char DAT_dos;
char DAT_GPS;
float flat, flon;
unsigned long age;
char coordenada_gps;
char datosSERIAL[30];

char clave_uno[]={'L','E','D','O','N'};//AQUI CAMBIAMOS TODO EL MENSAJE DE CONTROL
char clave_dos[]={'L','E','D','O','F'};//AQUI CAMBIAMOS TODO EL MENSAJE DE CONTROL
char clave_gps[]={'G','P','S'};

void setup()
{
  
  pinMode(led,OUTPUT);
  pinMode(estado,OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600); //modulo GSM
  Serial2.begin(9600); //modulo GPS
  
  configuracion_inicial();
  Serial.println("sistema de alarma GPS + SIM900 encendida");
  Serial.println("esperando recibir mensaje...");
  delay(1000);
}
/////////////////////////
void loop()
{
  leer_mensaje(); 
}
///////////////////////
void configuracion_inicial()//configura los codigos de lectura de mensajes
{
  Serial1.println("AT+IPR=9600");
  Serial.println("AT+IPR=9600");
  delay(500);
  Serial1.println("AT+CMGF=1");//modo texto
  Serial.println("AT+CMGF=1");//modo texto
  delay(500);
  Serial1.println("AT+CMGR=?");//ACTIVAMOS CODIGO PARA RECIBIR MENSAJES
  Serial.println("AT+CMGR=?");//ACTIVAMOS CODIGO PARA RECIBIR MENSAJES
 delay(500);
  Serial1.println("AT+CNMI=2,2,0,0");//ACTIVAMOS PARA VER MENSAJES
  Serial.println("AT+CNMI=2,2,0,0");//ACTIVAMOS PARA VER MENSAJES
  
  delay(500);
  Serial.println("configuracion terminada");
  delay(400);
    
}
///////////////////
void leer_mensaje()
{
  salir:
    if (Serial1.available()>0)
    {
     
      char DAT =Serial1.read();
      if(DAT=='@'){
        digitalWrite(estado,HIGH);//led de testeo de mensaje
        Serial.println("llego dato");//para verificar si lo detecto
      
        while(true)//ingresa en un while para leer solo los codigos futuros que estan por llegar despues de la arroba
        {
          if (Serial1.available()>0)
           {//cierre del segundo if
               char DAT_dos =Serial1.read();//@LEDON enter
               datosSERIAL[j]= DAT_dos;//almacena en cadena de caracteres, suma de caracteres     
               j++;
               //////////////////////////
               if (DAT_dos=='\n')//cuando termine de entregar todos los datos dara un enter
               //garantizando el final del codigo
                {
                 
                 Serial.println("Lectura correcta del codigo enviado:");//IMPRIME LOS CARACTERES ALMACENADOS PARA VER SI TODO ESTA OK
                 for(int i=0;i<=j;i++)
                  {
                 Serial.print(datosSERIAL[i]);//IMPRIME TODO EL CODIGO GUARDADO EN EL ARRAY
                  }
                  led_on();//llama a la la funcion y verifica codigo
                  led_off();//llama ala funcion para ver si es de apagar el led
                  digitalWrite(estado,LOW); //apaga el led despues de verificar
                  
                  for(int i=0;i<=j;i++)
                  {
                   datosSERIAL[i]==0;//borro array
                   DAT_dos=0;
                   DAT=0;
                  }
                  j=0;//borra el puntero o acumulador si no se hace esto no detecta los sigueintes codigos
                  goto salir;//sale de todos los ciclos y va al inicio para volver a leer codigo
                 }//CIERRA AL /N
               ////////////////////////     
            } //cierre del segundo if
          }//while
       }//arroba
    
    }//serial available 
  }  


void led_off()
{
  /////////////////////////
  if(datosSERIAL[0]==clave_dos[0] && datosSERIAL[1]==clave_dos[1] && datosSERIAL[2]==clave_dos[2] && datosSERIAL[3]==clave_dos[3] && datosSERIAL[4]==clave_dos[4])
   {
    digitalWrite(led,LOW);
    Serial.println(" LED  DESACTIVADO.....");
    mensaje_apagado();
   }
   else Serial.println(" codigo incorrecto led off");
                  
  
}

void led_on()
{
  /////////////////////////
  if(datosSERIAL[0]==clave_uno[0] && datosSERIAL[1]==clave_uno[1] && datosSERIAL[2]==clave_uno[2] && datosSERIAL[3]==clave_uno[3] && datosSERIAL[4]==clave_uno[4])
   {
    digitalWrite(led,HIGH);
    Serial.println(" LED ACTIVADO.....");
    mensaje_encendido();
   }
   else Serial.println(" codigo incorrecto led on");               
  
}

    void mensaje_apagado()
    {
      Serial1.println("AT+CMGF=1");//modo texto 
      //Imprime los datos al puerto serie como texto ASCII seguido de un retorno de carro
      delay(1000);
      Serial1.print("AT+CMGS=");// comando de envio de mensaje a un numero determinado
      delay(1000);
      //Imprime los datos al puerto serie como texto ASCII
      Serial1.print((char)34);//ponemos las comillas ", para que lo tome debe ser char de lo contrario el serial envia caracter por caracter
      Serial1.print(985819817);//colocamos numero de telefono
      Serial1.println((char)34);//volvemos a poner el caracter "
      delay(1000);//tiempo para que de respuesta el modulo >
      Serial1.print("led apagado");//mensaje que enviare
      Serial1.print((char)26);//ponemos el simbolo ascii 26,que corresponde a CTRL+Z,con lo que el modulo sabe que el sms termino
      
      Serial.print("led apagado");//mensaje que enviare
    }

    void mensaje_encendido()
    {
      Serial1.println("AT+CMGF=1");//modo texto 
      //Imprime los datos al puerto serie como texto ASCII seguido de un retorno de carro
      delay(1000);
      Serial1.print("AT+CMGS=");// comando de envio de mensaje a un numero determinado
      delay(1000);
      //Imprime los datos al puerto serie como texto ASCII
      Serial1.print((char)34);//ponemos las comillas ", para que lo tome debe ser char de lo contrario el serial envia caracter por caracter
      Serial1.print(985819817);//colocamos numero de telefono
      Serial1.println((char)34);//volvemos a poner el caracter "
      delay(1000);//tiempo para que de respuesta el modulo >
      Serial1.print("led encendido");//mensaje que enviare
      Serial1.print((char)26);//ponemos el simbolo ascii 26,que corresponde a CTRL+Z,con lo que el modulo sabe que el sms termino
      
      Serial.print("led encendido");//mensaje que enviare
    }
    ///////////////////////////////////
    /////////////////////////////////


    void mensaje_gps_coordenada()
    {
      Serial1.println("AT+CMGF=1");//modo texto 
      //Imprime los datos al puerto serie como texto ASCII seguido de un retorno de carro
      delay(1000);
      Serial1.print("AT+CMGS=");// comando de envio de mensaje  a un numero determinado
      delay(1000);
      //Imprime los datos al puerto serie como texto ASCII
      Serial1.print((char)34);//ponemos las comillas ", para que lo tome debe ser char de lo contrario el serial envia caracter por caracter
      Serial1.print(985819817);//colocamos numero de telefono
      Serial1.println((char)34);//volvemos a poner el caracter "
      delay(1000);//tiempo para que de respuesta el modulo >
      Serial1.print("LAT= ");
      Serial1.print(lati);
      Serial1.print("LON= ");
      Serial1.print(longi);
      Serial1.print((char)26);//ponemos el simbolo ascii 26,que corresponde a CTRL+Z,con lo que el modulo sabe que el sms termino
      delay(200);
      lati,longi=0;//borro los datos de la variable
      
    }
    ///////////////////////////////////

void gpsRead(){
  while (Serial2.available()){
      char c = Serial2.read();   //Se lee el puerto serial del GPS (Serial1) 
      if (gps.encode(c)){        //se pasa la variable C al gps 
         
         unsigned long age;
         gps.f_get_position(&flat, &flon, &age); //se le pide al GPS la posicion
         long speed=gps.f_speed_kmph();
         
         Serial.println("===============GPS==============");
         Serial.print("LAT=");      //Se muestran los datos en el serial del PC (SERIAL solo)
         Serial.print(flat, 6);
         Serial.print(" LON=");
         Serial.print(flon, 6);
         Serial.print(" velocidad=");
         Serial.print(speed, 6);
         Serial.println("===============GPS==============");
        
         //pasamos las variables para que las lea en codigo del GSM 
         lati=flat;
         longi=flon;
      }
}
}

