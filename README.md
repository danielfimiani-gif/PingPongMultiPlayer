# PingPongMultiPlayer

Trabajo práctico - Minijuego multijugador desarrollado en **Unreal Engine 5.6** con **Blueprints**.

Un Pong multijugador en red con sistema de sesiones, lobby, HUD replicado y manejo de
desconexiones. Ver la [consigna completa](docs/Consigna.md).

## Estado del proyecto

### Requisitos mínimos (aprobar)
- [ ] Menú principal (ingreso de nombre + crear / buscar sesión)
- [ ] Sala de lobby (host + invitados, sistema de "ready")
- [ ] Sistema de sesiones (Online Subsystem)
- [ ] Manejo de desconexiones según el jugador que se va

### Puntos adicionales
- [ ] Pantalla final con resultado
- [ ] Host puede kickear jugadores del lobby
- [ ] Menú de pausa con nombres y ping de jugadores

## Cómo abrir
Abrir `PingPongMultiPlayer.uproject` con Unreal Engine 5.6.

## Tecnología
- Unreal Engine 5.6 (Blueprints)
- Online Subsystem (sesiones)
- Git LFS para los assets binarios (`.uasset`, `.umap`, texturas, audio, etc.)
