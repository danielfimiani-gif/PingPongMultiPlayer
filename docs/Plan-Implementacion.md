# Plan de Implementación — Pong Multiplayer 1v1

> Documento de trabajo. Se va tildando a medida que avanzamos.
> Stack: UE 5.6, Blueprints, Online Subsystem (NULL para dev / Steam para demo).
> Sesiones: **Advanced Sessions plugin** (a confirmar con lo visto en clase).

---

## 1. Concepto del juego (1v1)

Pong clásico en red para **2 jugadores**:
- Dos paletas verticales, una a cada lado de la cancha. El **host** juega a la izquierda,
  el **cliente** a la derecha (asignado por el server).
- Una pelota rebota en las paredes superior/inferior y en las paletas.
- Si la pelota pasa una paleta y sale por ese lado, **el jugador del lado opuesto suma un punto**.
  La pelota se resetea al centro y sale hacia un lado al azar.

### Condición de victoria / derrota
La consigna pide **mínimo 5 minutos** de gameplay y un final claro (victoria/derrota):
- **Timer de partida: 5:00** (cuenta regresiva, replicada).
- Al llegar a **0:00**, gana quien tiene **más puntos**.
- **Empate** → muerte súbita: el **siguiente punto gana** (garantiza W/L claro).

Esto asegura los ≥5 min y una condición de fin inequívoca.

---

## 2. Mapas (Levels)

| Mapa | Rol | Networking |
|---|---|---|
| `L_MainMenu` | Menú principal: nombre + crear/buscar sesión | Standalone (sin red) |
| `L_Lobby` | Sala: host + invitado, estado ready, botón start / kick | **Listen server** |
| `L_Game` | Cancha de Pong 1v1 | Listen server (replicado) |

**Flujo:** MainMenu → (crear/unir sesión) → Lobby (listen server) → (todos ready + start) →
`ServerTravel` a L_Game → fin de partida → pantalla final → volver a MainMenu.

---

## 3. Arquitectura de clases (Blueprints)

### `BP_GameInstance` (persiste entre mapas)
- **Vars:** `PlayerName` (String), resultados de búsqueda de sesiones.
- **Responsable de:** crear / buscar / unir / destruir sesión (nodos de Advanced Sessions).
- Guarda el nombre ingresado en el menú para usarlo en todos los mapas.

### `BP_GameMode_Lobby` (solo server, mapa Lobby)
- Maneja login/logout de jugadores (PostLogin / Logout) → actualiza la lista.
- Valida "todos ready" para habilitar el Start.
- Ejecuta el `ServerTravel` a `L_Game` cuando el host arranca.
- Extra: kickear jugador.

### `BP_GameMode_Game` (solo server, mapa Game)
- Spawnea la pelota y asigna las paletas (izq/der).
- Lleva el **timer de 5 min** y detecta la condición de fin.
- Autoridad de las reglas: procesa goles, resetea pelota, decide ganador.
- Maneja desconexiones durante la partida.

### `BP_GameState` (replicado — visible por todos)
- `Score_P1`, `Score_P2` (RepNotify → actualiza HUD).
- `MatchTimeRemaining` (replicado).
- `MatchState` (WaitingToStart / Countdown / Playing / Finished).

### `BP_PlayerState` (replicado — uno por jugador)
- `PlayerDisplayName` (String, replicado — el nombre elegido).
- `bIsReady` (bool, replicado).
- Ping: se lee con el nodo de PlayerState (para el menú de pausa, punto extra).

### `BP_PlayerController`
- Maneja el input local de la paleta.
- Abre/cierra menú de pausa.
- Recibe RPCs Client (ej: "host desconectado", "sos el ganador").

### `BP_Paddle` (Pawn, replicado)
- La paleta que controla cada jugador. Movimiento arriba/abajo.
- Input local → mueve con autoridad correcta (server valida / replica posición).

### `BP_Ball` (Actor, replicado)
- La pelota. **Solo el server** simula su física/movimiento; se replica a los clientes.

### Widgets (UMG)
- `WBP_MainMenu` — input de nombre, botones Crear / Buscar, lista de sesiones.
- `WBP_Lobby` — lista de jugadores con nombre + ready, botón Ready, botón Start (host), Kick (host).
- `WBP_HUD` — nombres, marcador, tiempo restante (replicado en tiempo real).
- `WBP_Pause` — Reanudar / Salir al menú + (extra) lista de jugadores con ping.
- `WBP_EndScreen` — resultado final (Ganaste / Perdiste) + volver al menú.

---

## 4. Red: qué es RPC, qué es replicado, y autoridad

La consigna **valora especialmente** esto. Regla general:

- **Variable replicada (+ RepNotify):** para *estado continuo* que todos deben ver igual.
  Ej: puntajes, tiempo, nombres, ready, posición de la pelota.
- **RPC:** para *eventos puntuales*.
  - **Server RPC** (`Run on Server`): el cliente pide algo al server. Ej: "marcar ready",
    "mover paleta", "pedir kick" (si fuera el host).
  - **Client RPC** (`Run on owning Client`): el server le avisa algo a un cliente puntual.
    Ej: "fuiste kickeado", "host desconectado", "sos el ganador".
  - **Multicast** (`NetMulticast`): el server avisa a todos. Ej: efecto de gol, cuenta regresiva.
- **Autoridad (`HasAuthority` / `Switch Has Authority`):** la lógica de reglas (goles, timer,
  ganador, spawn de pelota) corre **solo en el server**. Los clientes mandan intención por RPC
  y reciben el estado por replicación. Nunca confiar en el cliente para el estado del juego.

---

## 5. Roadmap por milestones

### Milestone 0 — Setup ✅
- [x] Git + LFS + docs
- [x] Online Subsystem configurado (NULL/Steam toggle)
- [ ] Instalar Advanced Sessions plugin (5.6)

### Milestone 1 — Sesiones + Menú principal (REQUISITO MÍN.)
- [ ] `BP_GameInstance` con lógica de sesiones (crear/buscar/unir)
- [ ] `L_MainMenu` + `WBP_MainMenu`: input de nombre, Crear, Buscar+lista, Unir
- [ ] Al crear → host viaja a `L_Lobby`; al unir → cliente entra al lobby

### Milestone 2 — Lobby (REQUISITO MÍN.)
- [ ] `L_Lobby` como listen server + `BP_GameMode_Lobby`
- [ ] `WBP_Lobby`: lista de jugadores con nombre (replicado vía PlayerState)
- [ ] Estado **ready** por jugador (Server RPC + var replicada), host no marca ready
- [ ] Botón **Start** del host habilitado solo si todos ready
- [ ] Al start: asegurar que todos cargaron el nivel → `ServerTravel` a `L_Game`

### Milestone 3 — Gameplay 1v1 (REQUISITO parcial + HUD)
- [ ] `L_Game` + `BP_GameMode_Game` + `BP_GameState`
- [ ] `BP_Paddle` (input + replicación), `BP_Ball` (simulada por server)
- [ ] Sistema de puntaje + reset de pelota (autoridad server)
- [ ] Timer de 5 min + cuenta regresiva inicial
- [ ] `WBP_HUD` replicado: nombres, marcador, tiempo
- [ ] Condición de victoria/derrota (+ muerte súbita en empate)

### Milestone 4 — Pausa + Desconexiones (REQUISITO MÍN.)
- [ ] `WBP_Pause`: Reanudar / Volver al menú (sin frenar la simulación)
- [ ] Desconexión de un cliente → actualizar estado (Logout en GameMode)
- [ ] Si queda solo el host → termina la partida con mensaje "faltan jugadores"
- [ ] Si se desconecta el host → clientes reciben "host desconectado" y vuelven al menú

### Milestone 5 — Puntos extra
- [ ] `WBP_EndScreen`: pantalla final con resultado
- [ ] Host puede **kickear** en el lobby
- [ ] Menú de pausa muestra **nombres + ping** de los jugadores
- [ ] Activar y validar **Steam** (demo con compañero)

---

## 6. Cómo instalar Advanced Sessions (pendiente de confirmar)

Para un proyecto **BP puro** en 5.6 hay que usar una build **precompilada** del plugin
(el proyecto no compila C++). Opciones:
1. **Fab / Marketplace** (gratis): buscar "Advanced Sessions Plugin", instalar para 5.6
   en el engine. Luego habilitarlo en Edit → Plugins.
2. **GitHub** (mordentral/AdvancedSessions): descargar release de 5.6, copiar a
   `PingPongMultiPlayer/Plugins/AdvancedSessions/` (debe traer binarios precompilados).

> ⚠️ Bug conocido de 5.6: el join por **Steam** puede caer al IpNetDriver.
> No afecta a **NULL/LAN** (donde desarrollamos todo). Para la demo Steam se aplica el
> workaround: `steam_appid.txt` (con `480`) en `Binaries/Win64/` + flags ya puestas en
> `DefaultEngine.ini`. Detalle en [Multiplayer-Setup.md](Multiplayer-Setup.md).
