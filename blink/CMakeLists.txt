cmake_minimum_required(VERSION 3.20)
project(blink C)
# ============ HIER ANPASSEN ============
set(MCU atmega32) # Der Controllertyp
set(F_CPU 8000000UL) # 8 MHz - der Quarz auf dem Board
set(PORT COM6) # <<< DEINE Portnummer eintragen!
set(PROG stk500v2) # Das Protokoll des PROG-S
set(LFUSE 0xff)
set(HFUSE 0xd9)
# =======================================
set(CMAKE_C_STANDARD 99)
# Optionen fuer den Compiler
add_compile_options(
        -mmcu=${MCU} # Fuer welchen Chip uebersetzt wird
        -DF_CPU=${F_CPU} # Taktfrequenz an das Programm weitergeben
        -Os # Auf kleine Programmgroesse optimieren
        -Wall -Wextra # Alle Warnungen anzeigen
        -ffunction-sections -fdata-sections
)
# Optionen fuer den Linker
add_link_options(
        -mmcu=${MCU}
        -Wl,--gc-sections # Ungenutzten Code entfernen
        -Wl,-Map=${PROJECT_NAME}.map
)
# Das eigentliche Programm
add_executable(${PROJECT_NAME}.elf main.c)
# Nach dem Uebersetzen: HEX-Datei erzeugen und Groesse anzeigen
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND avr-objcopy -O ihex -R .eeprom
        $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_COMMAND} -E echo "Device: ${MCU}"
        COMMAND avr-size --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>
        COMMENT "HEX-Datei erzeugen"
)
# --- Zusaetzliche Knoepfe ---
add_custom_target(flash
        COMMAND avrdude -c ${PROG} -P ${PORT} -p m32
        -U flash:w:${PROJECT_NAME}.hex:i
        DEPENDS ${PROJECT_NAME}.elf
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Programm auf den Chip laden"
)
add_custom_target(fuses
        COMMAND avrdude -c ${PROG} -P ${PORT} -p m32
        -U lfuse:w:${LFUSE}:m -U hfuse:w:${HFUSE}:m
        COMMENT "Fuses setzen (8-MHz-Quarz)"
)
add_custom_target(readfuses
        COMMAND avrdude -c ${PROG} -P ${PORT} -p m32
        -U lfuse:r:-:h -U hfuse:r:-:h
        COMMENT "Fuses auslesen"
)
add_custom_target(testconnection
        COMMAND avrdude -c ${PROG} -P ${PORT} -p m32 -v
        COMMENT "Verbindung zum Chip pruefen"
)