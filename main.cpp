#include "mbed.h"
#include "arm_book_lib.h"

//#define ORIGINAL_CODE 1
#define BUSIN_CODE 1
//#define PORTIN_CODE 1

#define PORT_E_MASK 0x2A00
#define PORT_F_MASK 0xE000
#define GAS_DETECTOR_MASK 0x8000
#define OVER_TEMP_DETECTOR_MASK 0x2000
#define PORT_E_BUTTONS_MASK  0xA00
#define PORT_F_BUTTONS_MASK 0x6000

int main()
{
#ifdef ORIGINAL_CODE

    printf("Estoy usando ORIGINAL_CODE.\n");

    //!< @note Codigo original:
    DigitalIn gasDetector(D2);
    DigitalIn overTempDetector(D3);
    DigitalIn aButton(D4);
    DigitalIn bButton(D5);
    DigitalIn cButton(D6);
    DigitalIn dButton(D7);

    DigitalOut alarmLed(LED1);

    gasDetector.mode(PullDown);
    overTempDetector.mode(PullDown);
    aButton.mode(PullDown);
    bButton.mode(PullDown);
    cButton.mode(PullDown);
    dButton.mode(PullDown);

    alarmLed = OFF;

    bool alarmState = OFF;

    while (true) {

        if ( gasDetector || overTempDetector ) {
            alarmState = ON;
        }

        alarmLed = alarmState;

        
        if ( aButton && bButton && !cButton && !dButton) {
            alarmState = OFF;
        }
    }

#elif defined BUSIN_CODE

    printf("Estoy usando BUSIN_CODE.\n");

    //!< @note Uso la clase BusIn para los botones a (D4), b (D5), c (D6), d (D7)
    BusIn botones(D4, D5, D6, D7); //!< La mascara estara dada por D7 D6 D5 D4
    botones.mode(PullDown);

    //!< @note Uso la clase BusIn para los detectores gasDetector y overTempDetector
    BusIn detectores(D2, D3); //!< La mascara estara dada por D3 D2
    detectores.mode(PullDown);

    DigitalOut alarmLed(LED1);

    alarmLed = OFF;

    bool alarmState = OFF;

    while (true) {

        if ( detectores.read() == 1 || detectores.read() == 2 || detectores.read() == 3  ) {
            alarmState = ON;
        }

        alarmLed = alarmState;

        
        if ( botones.read() == 3) {
            alarmState = OFF;
        }
    }


#elif defined PORTIN_CODE

    printf("Estoy usando PORTIN_CODE.\n");

    /**
    * @note Para el uso de la clase PortIn:
    * gasDetector D2 = PF_15
    * overTempDetector D3 = PE_13
    * aButton D4 = PF_14
    * dButton D7 = PF_13
    * bButton D5 = PE_11
    * cButton D6 = PE_9
    */

    PortIn portE(PortE, PORT_E_MASK);
    PortIn portF(PortF, PORT_F_MASK);

    portE.mode(PullDown);
    portF.mode(PullDown);

    DigitalOut alarmLed(LED1);

    alarmLed = OFF;

    bool alarmState = OFF;

    while (true) {

        if ( (portF.read() & GAS_DETECTOR_MASK) != 0 || (portE.read() & OVER_TEMP_DETECTOR_MASK) != 0 ) {
            alarmState = ON;
        }

        alarmLed = alarmState;

        
        if ( (portE.read() & PORT_E_BUTTONS_MASK) != 0 || (portF.read() & PORT_F_BUTTONS_MASK) != 0 ) {
            alarmState = OFF;
        }
    }

    // Puede verse que en este caso, donde los detectores y los botones estan "mezclados" en diferentes puertos, es mas claro el uso de BusIn, ya que resulta un codigo mas facil de entender y por ende, de mantener.
    // Para poder aprovechar mejor la clase PortIn, deberiamos reubicar los detectores y los botones para que esten en los mismos puertos.
    // Como los detectores y los botones no estan en el mismo puerto se pueden notar las siguientes desventajas:
    //  - Nombres de los objetos no representativos (portE y portF)
    //  - Uso de mascaras para poder discriminar entre los detectores y los botones (Tanto en el port F como en el port E no hay solamente botones o detectores, estan mezclados)

#endif
}