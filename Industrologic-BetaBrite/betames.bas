100 REM  ------------------------------------------------------------
110 REM  BETAMES.BAS
120 REM  SIMPLE MESSAGE SOFTWARE FOR BETABRITE DISPLAY
130 REM -------------------------------------------------------------
140 TEMP1$="~1Special!, ~2On sale today and tomorrow only, "
150 TEMP2$="~5Widgets for only, ~1*** 9.99 ***, ~3What a cool deal!"
200 KEY OFF:FOR A=1 TO 10:KEY A,"":NEXT
210 CLS
220 PRINT "BetaBrite display message entry  version 10/18/01  Industrologic, Inc."
230 PRINT
240 PRINT "F1  = COM1"
250 PRINT "F2  = COM2"
260 PRINT "ESC = Exit program"
270 B$=INKEY$:IF B$="" THEN 270
280 IF B$=CHR$(0)+CHR$(59) THEN OPEN "COM1: 2400,E,7,1,CS,DS,CD" AS 1:GOTO 500
290 IF B$=CHR$(0)+CHR$(60) THEN OPEN "COM2: 2400,E,7,1,CS,DS,CD" AS 1:GOTO 500
300 IF B$=CHR$(27) THEN COLOR 7,0:CLS:END
310 GOTO 270
490 REM -----
500 CLS
510 PRINT "BetaBrite display message entry":PRINT
520 PRINT "Enter text to display"
530 PRINT "Use up and down arrow to select line 1 or 2"
540 PRINT "ESC = exit program":PRINT
550 PRINT "Use the following character pairs to change colors:"
560 PRINT "~1=red ~2=green ~3=amber ~4=orange ~5=yellow ~6=rainbow ~7=mixed colors"
570 LOCATE 10,1:PRINT "1: ";TEMP1$:LOCATE 11,1:PRINT "2: ";TEMP2$
580 LOCATE 13,1:PRINT "Resetting display...";
590 GOSUB 2000:REM RESET DISPLAY
600 T=TIMER
605 IF TIMER<T+3 THEN 605
607 LOCATE 13,1:PRINT "                         ";
609 REM -----
610 LOCATE 13,1:PRINT "Hit ENTER for line 2     "
615 TEMP$=TEMP1$
620 ROW=10:COL=4:LNG=70:GOSUB 10000:REM EDIT MESSAGE
630 TEMP1$=TEMP$
640 IF EXIT$="DOWN" THEN 670
650 IF EXIT$="UP" THEN 610
660 IF EXIT$="ESC" THEN COLOR 7,0:CLS:END
670 LOCATE 13,1:PRINT "Hit ENTER to send message"
675 TEMP$=TEMP2$
680 ROW=11:COL=4:LNG=70:GOSUB 10000:REM EDIT MESSAGE
690 TEMP2$=TEMP$
700 IF EXIT$="UP" THEN 610
710 IF EXIT$="DOWN" THEN 670
720 IF EXIT$="ESC" THEN COLOR 7,0:CLS:END
730 TEXT$=TEMP1$+TEMP2$
800 FOR X=1 TO 100
810   I=INSTR(TEXT$,"~1"):IF I<>0 THEN MID$(TEXT$,I,2)=CHR$(28)+"1"
820   I=INSTR(TEXT$,"~2"):IF I<>0 THEN MID$(TEXT$,I,2)=CHR$(28)+"2"
830   I=INSTR(TEXT$,"~3"):IF I<>0 THEN MID$(TEXT$,I,2)=CHR$(28)+"3"
840   I=INSTR(TEXT$,"~4"):IF I<>0 THEN MID$(TEXT$,I,2)=CHR$(28)+"7"
850   I=INSTR(TEXT$,"~5"):IF I<>0 THEN MID$(TEXT$,I,2)=CHR$(28)+"8"
860   I=INSTR(TEXT$,"~6"):IF I<>0 THEN MID$(TEXT$,I,2)=CHR$(28)+"9":REM OR A
870   I=INSTR(TEXT$,"~7"):IF I<>0 THEN MID$(TEXT$,I,2)=CHR$(28)+"B"
880 NEXT X
890 LOCATE 13,1:PRINT "Sending message...       ";
895 IF LEN(TEXT$)>13 THEN MODE$="a" ELSE MODE$="b"
900 GOSUB 1000:REM DISPLAY MESSAGE
910 T=TIMER
920 IF TIMER<T+2 THEN 920
930 LOCATE 13,1:PRINT "                         ";
940 GOTO 610
990 REM ----- WRITE MSG FILE "A" AND DISPLAY IT, LINE NUMBER "1" -----
1000 REM  TEXT$ - TEXT TO BE DISPLAYED, INCLUDING CONTROL STRINGS
1010 FOR A=1 TO 20:PRINT #1,CHR$(0);:NEXT A:REM LET DISPLAY DETERMINE BAUD RATE
1020 REM  "A" IS WRITE/DISPLAY TEXT, "A" IS MESSAGE ID "A", SPACE IS MID LINE?
1030 PRINT #1,CHR$(1)+"Z00"+CHR$(2)+"AA"+CHR$(27)+" ";
1040 REM  "a"=rotate,"b"=hold,"c"=flash,"d"=n/a (reserved),
1050 REM  "e"=roll up,"f"=roll down,"g"=roll left,"h"=roll right,"i"=wipe up,
1060 REM  "j"=wipe down,"k"=wipe left,"l"=wipe right,"m"=scroll,
1070 REM  "n"=special (add byte below to specify which),"o"=auto,"p"=roll in,
1080 REM  "q"=roll out,"r"=wipe in,"s"=wipe out,"t"=compressed rotate.
1500 PRINT #1,MODE$;:REM SET DISPLAY MODE
1510 PRINT #1,TEXT$;:REM DISPLAY TEXT
1520 PRINT #1,CHR$(4);:REM COMMAND SUFFIX, CAN END ON EOT
1530 RETURN
1990 REM ----- RESET DISPLAY -----
2000 FOR A=1 TO 20:PRINT #1,CHR$(0);:NEXT A:REM LET DISPLAY DETERMINE BAUD RATE
2010 PRINT #1,CHR$(1)+"Z00"+CHR$(2)+"E$"+CHR$(4);:REM clear memory config, reset
2020 RETURN
9930 REM *********************************************************************
9935 REM ---------------- EDIT A FIELD AT ROW AND COLUMN ---------------------
9940 REM
9945 REM This subroutine allows you to edit or enter a string. The editing
9950 REM keys work about the same as BASIC in the screen editing mode.
9955 REM
9960 REM enter with - ROW and COL=row and column at which to edit the string
9965 REM              LNG=maximum length of string allowed
9970 REM              TEMP$="string" to edit
9975 REM exit with -  TEMP$="string" edited or entered
9980 REM              EXIT$=key hit that exited routine, "ENTER" or "ESC"
9985 REM              ESCAPE=-1 (TRUE) if ESCAPE key hit
9990 REM              (for compatibility with old versions)
9995 REM
10000 LOCATE ,,0:INS=0:TEMP$=LEFT$(TEMP$,LNG)
10005 IF LEN(TEMP$)=LNG THEN EDCOL=COL+LEN(TEMP$)-1 ELSE EDCOL=COL+LEN(TEMP$)
10010 LOCATE ROW,COL,0:COLOR 0,7:PRINT TEMP$;
10015 LOCATE ROW,COL+LEN(TEMP$):PRINT SPACE$(LNG-LEN(TEMP$));
10020 L=LEN(TEMP$)
10025 AAAA$=MID$(TEMP$,EDCOL-COL+1,1)
10030 IF INS THEN LOCATE ROW,EDCOL:COLOR 15,0
10035 IF INS THEN PRINT AAAA$;:COLOR 0,7
10040 IF EDCOL=COL+LNG THEN EDCOL=EDCOL-1
10045 LOCATE ROW,EDCOL
10050 LOCATE,,1
10055 IN$=INKEY$:IF IN$="" THEN 10055                        :REM WAIT FOR KEY
10060 IF IN$=CHR$(13) THEN EXIT$="ENTER":ESCAPE=0:GOTO 10325 :REM ENTER
10061 IF IN$=CHR$(27) THEN EXIT$="ESC":ESCAPE=-1:GOTO 10325  :REM ESCAPE
10062 IF IN$=CHR$(0)+CHR$(72) THEN EXIT$="UP":ESCAPE=0:GOTO 10325
10063 IF IN$=CHR$(0)+CHR$(80) THEN EXIT$="DOWN":ESCAPE=0:GOTO 10325
10070 IF IN$=CHR$(8) THEN 10180                              :REM BACKSPACE
10075 IF IN$=CHR$(0)+CHR$(75) THEN 10225                     :REM LEFT ARROW
10080 IF IN$=CHR$(0)+CHR$(77) THEN 10240                     :REM RIGHT ARROW
10085 IF IN$=CHR$(0)+CHR$(79) THEN 10275                     :REM END
10090 IF IN$=CHR$(0)+CHR$(82) THEN 10295                     :REM INSERT
10095 IF IN$=CHR$(0)+CHR$(83) THEN 10305                     :REM DELETE
10100 REM ----- allow only legitimate alphanumeric
10105 IF ASC(IN$)<32 OR ASC(IN$)>126 THEN GOSUB 10400:GOTO 10050
10110 REM ----- good character
10115 L=EDCOL-COL:IF INS THEN 10155
10120 REM ----- add character if at end of string
10125 IF EDCOL=COL+LEN(TEMP$) THEN TEMP$=TEMP$+IN$:EDCOL=EDCOL+1:GOTO 10010
10130 REM ----- add character in middle if not in insert mode
10135 TEMP$=LEFT$(TEMP$,L)+IN$+RIGHT$(TEMP$,LEN(TEMP$)-1-L):EDCOL=EDCOL+1
10140 IF EDCOL=COL+LNG THEN GOSUB 10400   :REM beep if typing over last char.
10145 GOTO 10010
10150 REM ----- in insert mode, check for full field
10155 IF LEN(TEMP$)=LNG THEN GOSUB 10400:GOTO 10010        :REM full field
10160 TEMP$=LEFT$(TEMP$,L)+IN$+RIGHT$(TEMP$,LEN(TEMP$)-L)  :REM not full
10165 EDCOL=EDCOL+1:GOTO 10010
10170 REM ----- BACKSPACE -----
10175 REM ----- error if at leftmost column
10180 IF EDCOL=COL THEN GOSUB 10400:GOTO 10010
10185 REM ----- skip moving cursor left if the field is full
10190 IF LEN(TEMP$)=LNG AND EDCOL=COL+LNG-1 THEN 10215
10195 REM ----- normal backspace (in middle of field)
10200 L=EDCOL-1-COL:TEMP$=LEFT$(TEMP$,L)+RIGHT$(TEMP$,LEN(TEMP$)-1-L)
10205 EDCOL=EDCOL-1:GOTO 10010
10210 REM ----- backspace if at rightmost column
10215 TEMP$=LEFT$(TEMP$,LEN(TEMP$)-1):GOTO 10010
10220 REM ----- LEFT ARROW -----
10225 INS=0:IF EDCOL>COL THEN EDCOL=EDCOL-1:GOTO 10010
10230 GOSUB 10400:GOTO 10010                           :REM error if left col
10235 REM ----- RIGHT ARROW -----
10240 INS=0:IF EDCOL<COL+LNG-1 THEN 10255
10245 GOSUB 10400:GOTO 10010                           :REM error if right col
10250 REM ----- move cursor right if applicable
10255 IF EDCOL<COL+LEN(TEMP$) THEN EDCOL=EDCOL+1:GOTO 10010
10260 REM add "blank" character when cursor is moved to right
10265 TEMP$=TEMP$+CHR$(255):EDCOL=EDCOL+1:GOTO 10010
10270 REM ----- END -----
10275 INS=0:IF LEN(TEMP$)=LNG THEN EDCOL=COL+LEN(TEMP$)-1:GOTO 10010
10280 EDCOL=COL+LEN(TEMP$):GOTO 10010
10285 REM:END:IF EDCOL>COL+LNG THEN EDCOL=COL+LNG:GOTO 10025
10290 REM ----- INSERT -----
10295 IF INS THEN INS=0:GOTO 10010 ELSE INS=-1:GOTO 10010
10300 REM ----- DELETE -----
10305 INS=0:L=EDCOL-COL
10310 IF EDCOL>=COL+LEN(TEMP$) THEN GOSUB 10400:GOTO 10010
10315 TEMP$=LEFT$(TEMP$,L)+RIGHT$(TEMP$,LEN(TEMP$)-L-1):GOTO 10010
10320 REM ----- COMMON EXIT -----
10325 REM remove blanks at end
10330 IF RIGHT$(TEMP$,1)=CHR$(255) THEN TEMP$=LEFT$(TEMP$,LEN(TEMP$)-1):GOTO 10330
10335 REM change blanks in middle to spaces
10340 FOR AAAA%=1 TO LEN(TEMP$)
10345 IF MID$(TEMP$,AAAA%,1)=CHR$(255) THEN MID$(TEMP$,AAAA%,1)=" "
10350 NEXT AAAA%
10355 REM redisplay string
10360 LOCATE ROW,COL,0:COLOR 7,0
10365 PRINT TEMP$+SPACE$(LNG-LEN(TEMP$));
10370 RETURN
10375 REM
10395 REM ----- BEEP IF ERROR -----
10400 BEEP:RETURN
10405 REM

