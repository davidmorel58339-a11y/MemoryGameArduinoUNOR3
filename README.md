# JuegoDeMemoriaArduinoUNOR3
Todo lo necesario para hacer un juego Simon Says con un arduino UNO R3, incluido el diseño de la carcasa y el diseño de la PCB
Me basé en el video "How to Make Simon Memory Game Using Arduino | Simon Says Game"
https://youtu.be/8ORj7vU1hU0?si=2jYOV7qwlNEzzdiu

Le hice algunas modificaciones al código:
- Si se presiona cualquier otro botón que no sea el rojo mientras la pantalla presenta "Press red button...", se reproduce un fragmento de jingle bells.
- Cuando pierdes en un nivel, se reproduce un fragmento del Game Over Super Mario.
IMPORTANTE:
Para correr el código con estas modificaciones y que no salte error al tratar de subirlo al Arduino, hay que descargarse la librería que incluí en la carpeta del código, se llama "pitches", y define la frecuencia de las notas musicales para el buzzer.

Le hice algunas modificaciones al circuito:
- Le conecté la batería de 9v al Jack de alimentación del Arduino (7v-12v).
- Esta conexión entre el jack y la pila, la interrumpí con un interruptor de dos posiciones (listado en el BOM).
- Como los leds van fuera de la PCB, coloqué terminales de bloque en vez de los LEDs.
