; Map one of the unused auxiliary IO ports to act as a row reporter
; through the use of libgme_M. First, write the data, then activize it
; with the cmd (love making words up recently).
.equ reportTrackerCmd $f8
.equ reportTrackerArg $f9

.enum 0
  reportTrackerCmd_IncRow     db
	reportTrackerCmd_SetRow     db
	reportTrackerCmd_SetPattern db
.ende

; compress bits for patterns
.define CBIT 7
.define CBIT_NOTE 0
.define CBIT_INSTR 1
.define CBIT_VOL 2
.define CBIT_FX 3
.define CBIT_FXPARAM 4
.define CBIT_RLE 5
.define CBIT_RLE_ONLY1 6

; extflags (defined in .s)
.enum 0
	REPEATPATTERN db
.ende

; Generate C printouts for the Tracker app
.PRINT "#define CBIT 0x", HEX CBIT, "\n"
.PRINT "#define CBIT_NOTE 0x", HEX CBIT_NOTE, "\n"
.PRINT "#define CBIT_INSTR 0x", HEX CBIT_INSTR, "\n"
.PRINT "#define CBIT_VOL 0x", HEX CBIT_VOL, "\n"
.PRINT "#define CBIT_FX 0x", HEX CBIT_FX, "\n"
.PRINT "#define CBIT_FXPARAM 0x", HEX CBIT_FXPARAM, "\n"
.PRINT "#define CBIT_RLE 0x", HEX CBIT_RLE, "\n"
.PRINT "#define CBIT_RLE_ONLY1 0x", HEX CBIT_RLE_ONLY1, "\n"
.PRINT "\n"
.PRINT "#define REPORT_TRACKER_INCROW 0x", HEX reportTrackerCmd_IncRow, "\n"
.PRINT "#define REPORT_TRACKER_SETROW 0x", HEX reportTrackerCmd_SetRow, "\n"
.PRINT "#define REPORT_TRACKER_SETPATTERN 0x", HEX reportTrackerCmd_SetPattern, "\n"
.PRINT "\n"
.PRINT "#define EXTFLAGS_REPEATPATTERN 0x", HEX REPEATPATTERN, "\n"
; We can make struct definitions that don't need to be defined in RAM,
; used purely by their definition as a way to informatically load offsets
; from RAM pointers

.struct PatternSequencer
num_entries   db ;length in bytes (same as number of pattern entries)
sequences     db
.endst
/* sequences is described to assembler as 1 byte in size but that is just to
   make it addressable. In reality, it's the size specified by num_entries.
   Each entry is just a byte specifying the pattern number */

; it turns out that due to the use of indirect addressing, I can't make
; use of these struct definitions, but they still uphold the rules of the
; locations of the DATA members if I export them to C structs that the C
; code uses.
.struct Instrument
vol db
pan db ; TODO
srcn db
adsr1 db
adsr2 db
semitone_offset db ; TODO
finetune db        ; TODO
.endst
