# Setup de Multiplayer (Online Subsystem)

Este proyecto usa el **Online Subsystem** de Unreal para la gestión de sesiones.
La lógica de sesiones en Blueprints es **agnóstica al subsystem**: los mismos nodos
`Create Session` / `Find Sessions` / `Join Session` funcionan tanto en LAN (NULL) como
en Steam. Por eso cambiar de uno a otro es **solo config**, sin tocar Blueprints.

## Plugins habilitados
En `PingPongMultiPlayer.uproject`:
- `OnlineSubsystem`
- `OnlineSubsystemUtils`
- `OnlineSubsystemSteam`

## Toggle NULL ↔ Steam
En `Config/DefaultEngine.ini`, sección `[OnlineSubsystem]`, se cambia **una sola línea**:

```ini
[OnlineSubsystem]
DefaultPlatformService=Null    ; desarrollo en tu PC (PIE, varias ventanas)
; DefaultPlatformService=Steam ; demo / entrega con un compañero
```

### NULL (desarrollo)
- Sesiones **LAN**. Se puede testear todo en una sola PC abriendo varias ventanas
  (PIE con "Number of Players" > 1, o varias instancias Standalone).
- Iteración rápida, ideal para construir el juego.

### Steam (demo / entrega)
- Requiere **Steam abierto y logueado** en cada máquina.
- Usa el **AppId 480 (Spacewar)**, el appid de prueba oficial: permite que cualquiera
  se una sin necesidad de ser el dueño de un juego publicado.
- **Importante:** Steam permite una sola sesión por cuenta, así que para probar host +
  cliente necesitás **2 máquinas con 2 cuentas de Steam distintas** (o un compañero).
- No se puede testear Steam desde PIE: hay que correr builds **Standalone**.

## Flujo de trabajo recomendado
1. Desarrollar y testear todo con `DefaultPlatformService=Null` (rápido, una sola PC).
2. Para la entrega/demo, cambiar a `Steam`, empaquetar o correr Standalone, y validar
   host + cliente con el compañero.

## Buenas prácticas de red (lo que valora la consigna)
- **RPCs** (Server / Client / Multicast) para eventos puntuales (ej: marcar "ready",
  arrancar partida, notificar desconexión del host).
- **Variables replicadas** con `RepNotify` para estado continuo (vida, puntaje, nombres).
- **Autoridad**: la lógica de juego corre en el server (`HasAuthority`); los clientes
  solo envían intención vía RPC y reciben el estado replicado.
