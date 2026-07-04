# Parcial - Minijuego Multijugador

## Introducción
El parcial consiste en el desarrollo de un minijuego multijugador en donde el alumno
deberá desarrollarla utilizando las herramientas Multiplayer. Cada alumno creará un
minijuego libre que integre gestión de sesiones, sala de lobby, sincronización de estados,
gestión de jugadores con conexiones y desconexiones.

Hacer el minijuego requerirá un mínimo de puntos necesarios para aprobar (4). Los
puntos adicionales permitirán obtener una mejor calificación hasta alcanzar la nota
máxima (10).

## Formato de Entrega
Cada alumno deberá entregar un archivo ZIP con el proyecto o un enlace a un
repositorio donde se puedan ver los archivos creados. **No es necesario entregar una
build del juego.**

## Minijuego Libre

### Descripción
El minijuego debe arrancar mostrando un **menú principal** en el que el jugador, antes de
cualquier otra acción, ingrese un **nombre** que lo identifique durante toda la experiencia.
Desde ese mismo menú, el jugador tendrá la posibilidad de **crear una sesión** o de **buscar
y unirse** a una existente. Cuando un jugador crea la sesión, se convierte automáticamente
en **host** y queda ubicado dentro de una **sala de lobby**. A medida que otros jugadores se
unan, sus nombres aparecerán en ese espacio y cada uno, excepto el host, podrá marcar
o desmarcar su estado de **"ready"**. Solo cuando todos los participantes hayan señalado
que están listos, el host dispondrá de un botón para **arrancar la partida**, momento en el
cual el sistema deberá asegurarse de que cada cliente haya cargado correctamente el
nivel de gameplay antes de iniciar la cuenta regresiva.

La fase de **gameplay** tendrá una duración mínima de **cinco minutos** y culminará
necesariamente en una condición de **victoria o derrota**, establecida según las reglas del
juego que cada alumno defina. Durante la partida, en todo momento y de manera
**sincronizada** para todos los jugadores, deberá mostrarse un feedback claro con
información de estado. Ejemplo: el nombre de cada jugador, su barra de vida, la
puntuación o cualquier otro indicador de progreso relevante. Este **HUD** deberá
actualizarse en tiempo real y **replicarse** correctamente entre clientes para que cada
usuario tenga acceso inmediato a los cambios de estado de sus compañeros y rivales.

Además, el juego debe permitir **pausar** la acción sin detener la simulación del juego. Al
abrir el menú de pausa, el jugador podrá optar por **reanudar** la partida o **regresar al menú
principal**.

Por último, el sistema debe gestionar **desconexiones** de manera robusta:
- Si cualquier jugador abandona la partida, el estado del juego se actualizará para reflejar su ausencia.
- Si solo permanece el host, la partida finalizará automáticamente mostrando un mensaje que indique la falta de participantes antes de volver al menú.
- Si es el host quien se desconecta, todos los demás recibirán una notificación de **"host desconectado"** seguida del retorno al menú principal.

## Criterios de Evaluación

### Mínimo para aprobar (4 puntos):
- [ ] Implementación del **Menú Principal** para crear o buscar sesiones.
- [ ] Implementación de una **sala de Lobby**, en donde estén el Host y los invitados.
- [ ] Desarrollo del **Sistema de Sesiones**.
- [ ] Gestionar las **desconexiones** según qué jugador se desconecte (actualizar el estado del juego cuando pasa esto).

### Puntos adicionales:
- [ ] Agregar **pantalla final** al terminar la partida para mostrar el resultado del juego.
- [ ] Añadir opción al Host para que pueda **kickear** a un jugador en el Lobby.
- [ ] Mostrar en el **menú de pausa** la información de los jugadores (nombres y ping).

## Notas Finales
- Cada estudiante desarrollará un minijuego libre.
- Se aprueba con 4. Los puntos extra permiten llegar hasta 10.
- **Se valorará bastante la implementación de eventos RPC, uso correcto de variables Replicadas y buen manejo de Autoridad.**
