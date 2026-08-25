## Laborumgebung

Im [TILAB](http://www.tilab.tuwien.ac.at/) sind vergleichsweise moderne GNU/Linux-Desktop-PCs verfügbar. Die voreingestellte Shell (das Programm, das u.a. die Eingabeaufforderung zur Verfügung stellt) ist die <tt>bash</tt>; als PDF-Reader ist _Evince_ (<tt>evince</tt>) installiert. Bei Webbrowsern hat man die Wahl zwischen Firefox (<tt>firefox</tt>), Google Chrome (<tt>google-chrome</tt>), Opera (<tt>opera</tt>) sowie Konqueror (<tt>konqueror</tt>).

### Editoren

Für erfahrene Unix-Benutzer sind die Editoren [_vim_ und _gvim_](http://www.vim.org/) (<tt>vim</tt>, <tt>gvim</tt>), sowie [_Emacs_](http://www.gnu.org/software/emacs/) (<tt>emacs</tt>) installiert. Etwas einsteigerfreundlichere Editoren fürs Terminal sind [_pico_](http://www.washington.edu/pine/) (<tt>pico</tt>) und sein großer Bruder [_GNU nano_](http://www.nano-editor.org/) (<tt>nano</tt>); für die grafische Benutzeroberfläche dann [_gedit_](http://projects.gnome.org/gedit/) (<tt>gedit</tt>), [_KWrite_ und _Kate_](http://kate-editor.org/) (<tt>kwrite</tt>, <tt>kate</tt>).

### Fernzugriff

Auf die TILAB-Rechner ist auch der Zugriff von außen möglich; dies geschieht über das <abbr title="Secure Shell">SSH</abbr>-Protokoll. Unter Unix-artigen Systemen (z.B. Linux, Mac OS X) kann [OpenSSH](http://openssh.org/), das oftmals bereits vorinstalliert ist, verwendet werden; unter Windows bietet sich das Programm [PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/) an.

Der OpenSSH-Client lässt sich mit dem Befehl `ssh _benutzername_@ssh.tilab.tuwien.ac.at` starten. Der Benutzername lautet normalerweise `y_matrikelnummer_`. (Beispiel: `ssh y0925631@ssh.tilab.tuwien.ac.at`) Die Abfrage betreffend Authentizität kann meist getrost mit _yes_ beantwortet werden; das Kennwort, sofern nicht bereits geändert, steht am Abriss, der bei der Accountabholung ausgehändigt wurde. Bei der Eingabe des Kennwortes erscheinen **keine** Sternchen oder Punkte; das Terminal bleibt stumm, bis Enter gedrückt wurde. Wenn alles erfolgreich verlaufen ist, wird die Shell gestartet und es erscheint eine Eingabeaufforderung wie auf einem lokalen Terminal.

Bei PuTTY wird das Feld _Host Name (or IP Address)_ wie bei OpenSSH mit `_benutzername_@ssh.tilab.tuwien.ac.at` ausgefüllt; unter _Protocol_ ist _SSH_ zu wählen. Nach einem Klick auf _Open_ wird eine Verbindung zum Server aufgebaut und eine Kennwortabfrage angezeigt. Ab dort verhält sich PuTTY annähernd genauso wie OpenSSH.

Es ist egal, welcher der 34 Rechner gewählt wird. Der Inhalt des eigenen Home-Ordners, die eigenen Daten also, sind an einem zentralen Server gespeichert und werden von allen Rechnern gleichermaßen eingebunden. Eine Aufteilung ist trotzdem gewünscht: wenn sich mehrere Leute mit demselben Computer verbinden, ist er stärker ausgelastet und wird für alle eingeloggten User „langsamer“.

### Kennwortänderung

Das Kennwort kann im Terminal mit Hilfe des Programms <tt>passwd</tt> geändert werden. Dieses fragt zuerst nach dem bestehenden Kennwort und dann zweimal nach einem neuen. Bei der Eingabe sämtlicher Kennwörter bleibt das Terminal stumm; es erscheinen, wie bei SSH, **keine** Sternchen oder Punkte. Sollte man ein zu einfaches Kennwort gewählt haben, wird es mit einer entsprechenden Meldung abgewiesen.

## Ein einfaches C-Programm

Ein minimales C-Programm sieht wie folgt aus:

<pre>
int main(void)
{
    return 0;
}
</pre>

Die Funktion <tt>main</tt> hat eine Sonderstellung: sie wird im Programm als erste aufgerufen. Sie gibt eine Ganzzahl (<tt>int</tt>) zurück und nimmt zumindest in diesem Beispiel keine Argumente entgegen. (Aus historischen Gründen würden leere Parameter-Klammern bedeuten, dass die Funktion „irgendwas“ entgegen nimmt; das <tt>void</tt> ist notwendig, um zu unterstreichen, dass sie gar keine Parameter akzeptiert.) Der Rückgabewert ist 0, wenn das Programm erfolgreich ausgeführt wurde, und ein anderer Wert, falls ein Fehler auftrat (nach dem für Unix typischen Motto _ein Programm kann nur auf eine Art und Weise erfolgreich sein, aber auf abertausende Arten von Problemen stoßen_).

Das Programm wird nun in einer Datei namens <tt>cexample.c</tt> abgespeichert.

### Compiler-Aufruf

Der entsprechende Compiler-Aufruf sieht wie folgt aus:

`gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -o cexample cexample.c`

Diese lange Wurst besteht aus folgenden Zutaten:

*   <tt>gcc</tt>: der Name des Compilers ([GNU C Compiler](http://gcc.gnu.org/))
*   <tt>-std=c99</tt>: verwende den C99-Standard und keinen anderen (C90, C11, K&R C, usw.)
*   <tt>-pedantic</tt>: nimm den angegebenen Standard sehr ernst; gib eine Warnung aus, falls vom Standard nicht vorgesehene Features verwendet werden
*   <tt>-Wall</tt>: aktiviere alle sinnvollen Warnungen
*   <tt>-D_DEFAULT_SOURCE</tt>: aktiviert POSIX 2008 (leider versteckt <tt>-std=c99</tt> einige Funktionen, die für die Entwicklung unter Unix ganz nützlich sind; diese werden von diesen Parametern wieder aktiviert; siehe auch `man 7 feature_test_macros` oder [glibc Feature-Test-Macros](https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html))
*   <tt>-o cexample</tt>: die Datei, in die das kompilierte Programm gespeichert wird (hier <tt>cexample</tt>)
*   <tt>cexample.c</tt>: die Quelldateien des Programms

Es gibt einen Unterschied zwischen Fehlern (errors) und Warnungen (warnings): Fehler werden vom Compiler gemeldet, wenn der Code ungültig ist; Warnungen werden gemeldet, wenn der Code zwar gültig ist, aber „ich bin mir nicht sicher, dass du wirklich das gemeint hast, was du geschrieben hast“). Notizen (notes) enthalten weiterführende Informationen zu Fehlern oder Warnungen. Bitte beachten Sie, dass wir bei Ihren Programmen auf einer Kompilierung **ohne Fehlermeldungen oder Warnungen** bestehen. Versuchen Sie also bitte noch vor der Abgabe Ihr Programm in der TILab-Umgebung zu kompilieren (siehe auch den Abschnitt [Fernzugriff](#fernzugriff)).

Wenn der Compiler keine Meldung ausgibt, verlief die Kompilierung problemlos.

Danach kann das Programm mit <tt>./cexample</tt> ausgeführt werden – es gibt aber (noch) nichts aus.

### Hello, World!

Erweitern wir das Programm nun um eine textuelle Ausgabe.

<pre>
int main(void)
{
    printf("Hello, world!\n");
    return 0;
}
</pre>

Die Funktion <tt>printf</tt> erlaubt eine formatierte Ausgabe, kann aber auch für das einfache Ausgeben von Strings verwendet werden. Jedoch gibt der Compiler nun folgende Warnung aus:

<pre>
cexample.c: In function ‘main’:
cexample.c:3: warning: implicit declaration of function ‘printf’
cexample.c:3: warning: incompatible implicit declaration of built-in function ‘printf’
</pre>

Vor allem die dritte Zeile mutet an, dass der Compiler die Funktion kennt, wir aber noch eine entsprechende Header-Datei einbinden müssen. Nur wo finden wir diese Information?

## Manpages

Die Manpages (_manual pages_, haben also nichts mit Männern zu tun) sind das mitgelieferte Handbuch eines Unix-Systems. Darin sind unter anderem auch alle Funktionen der C-Bibliothek dokumentiert.

Wir versuchen also, die <tt>printf</tt>-Manpage mit dem Befehl <tt>man printf</tt> zu öffnen. Nach einer kurzen Lektüre stellt sich jedoch heraus, dass das nicht die erwünschte Seite ist – hier wird der Shell-Befehl <tt>printf</tt> beschrieben. In der Kopfzeile steht <tt>PRINTF(1)</tt>, also der Artikel _printf_ aus dem Buch 1 (dem Buch der Shell-Befehle). Funktionen, die für C relevant sind, befinden sich jedoch in den Büchern 2 (Systemaufrufe) und 3 (Bibliotheksaufrufe). Generell empfiehlt es sich, zuerst im Buch 2 und dann im Buch 3 nachzuschlagen.

<tt>man 2 printf</tt> findet nichts, also versuchen wir <tt>man 3 printf</tt> und kommen zur erwünschten Manpage. Oben steht <tt>#include <stdio.h></tt>; wir müssen also unseren Quellcode wie folgt anpassen:

<pre>
#include <stdio.h>

int main(void)
{
    printf("Hello, world!\n");
    return 0;
}
</pre>

Der Betrachter der Manpages ist <tt>less</tt> und unterstützt folgende Kommandos:

*   <tt>q</tt>: beenden
*   <tt>/_bla_</tt>: nach dem nächsten Vorkommen des Textes _bla_ suchen
*   <tt>n</tt>: zum nächsten Treffer springen
*   <tt>N</tt>: zum vorigen Treffer springen
*   <tt>?_bla_</tt>: wie <tt>/_bla_</tt>, nur rückwärts suchen (auch die Bedeutung von <tt>n</tt> und <tt>N</tt> wird vertauscht!)

Es ist vom Vorteil, sich ausgiebig mit den Manpages auseinanderzusetzen. Sie sind das **einzige erlaubte Hilfsmittel** bei den Tests!

Das Programm kompiliert nun ohne Warnungen und gibt, wie erwünscht, _Hello, world!_ aus.

## make

Natürlich nervt es, jedes Mal den ellenlangen Compileraufruf eingeben zu müssen, um zu einem ausführbaren Programm zu gelangen. Abhilfe schafft das Programm <tt>make</tt>, das ein „Kochrezept“ aus der Textdatei namens _Makefile_ einliest und abarbeitet.

Eine Makefile sieht im Allgemeinen wie folgt aus:

<pre>
variable1=wert1
variable2=wert2

# Kommentar

ziel1: quelle1 quelle2 quelle3
    befehl1
    befehl2

ziel2: quelle1 quelle4 quelle5
    befehl1
    befehl2
</pre>

Die minimale Makefile für <tt>cexample</tt> sieht wie folgt aus:

<pre>
cexample: cexample.c
    gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -o cexample cexample.c
</pre>

Ein einfacher Aufruf von <tt>make</tt> reicht, und das Programm wird kompiliert. (Falls die Datei _cexample_ neuer ist als die Datei _cexample.c_, macht <tt>make</tt> jedoch nichts, damit keine Zeit grundlos verbraten wird.)

Eine „gute“ Makefile stellt konventionell zumindest zwei (virtuelle) Ziele zur Verfügung: _all_ und _clean_. _all_ sorgt dafür, dass auch in einem Projekt mit mehr als einem Programm sämtliche Programme kompiliert werden. _clean_ löscht alle Produkte und Zwischenprodukte, die während des Kompilierungsprozesses erstellt wurden.

Wenn kein Argument an <tt>make</tt> übergeben wird, wird einfach das erste Ziel in der Makefile abgearbeitet. Es ist demnach empfohlen, _all_ als erstes Ziel anzuführen. Eine entsprechend erweiterte Makefile sieht wie folgt aus:

<pre>
all: cexample

cexample: cexample.c
    gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -o cexample cexample.c

clean:
    rm -f cexample
</pre>

(Das <tt>-f</tt> (_force_) im <tt>rm</tt>-Aufruf sorgt dafür, dass <tt>rm</tt> auch dann erfolgreich beendet wird, wenn die Datei, oder eine der Dateien, nicht (mehr) existiert.)

### Größere Projekte (Compiler vs. Linker)

Stellen wir uns nun ein Projekt vor, das aus drei Quelldateien besteht: _common.c_, _client.c_ und _server.c_. Daraus sollen nun zwei Binaries werden: _client_ und _server_, wobei neben der jeweils entsprechenden Quelldatei beide Binaries von _common.c_ abhängen.

Eine Makefile nach üblichem Schema würde wie folgt aussehen:

<pre>
all: client server

client: client.c common.c
    gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -o client client.c common.c

server: server.c common.c
    gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -o server server.c common.c

clean:
    rm -f client server
</pre>

Dies ist etwas ineffizient. Bereits ein einfaches <tt>#include <stdio.h></tt> bewirkt, dass der Text, den der Compiler „durchlesen und verstehen“ muss, um etwa 500 Zeilen länger wird. Wenn wir nun die Datei <tt>common.c</tt> zweimal kompilieren, geht damit viel Zeit verloren; außerdem wird sie jedes mal mitkompiliert, wenn sich die <tt>client.c</tt> oder die <tt>server.c</tt> ändern.

Um dieser Situation etwas entgegenzuwirken, ermöglichten die C-Entwickler eine zweistufige Kompilierung:

1. Die eigentliche Kompilierung. Aus den Quelldateien werden _Objektdateien_ erstellt, die bereits den kompilierten Programmcode, aber nur leere Verweise auf externe Funktionen enthalten.
2. Das _Linken_. Aus den Objektdateien wird eine ausführbare Binärdatei erstellt, indem der Code aus allen Objektdateien zusammengeführt und die Verweise auf externe Funktionen entsprechend angepasst werden.

Eigentlich ist genau das bereits früher passiert, aber <tt>gcc</tt> hatte sich darum gekümmert, dieses Detail weg zu abstrahieren. Die expliziten Aufrufe des Compilers und des Linkers sehen wie folgt aus:

<pre>
gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -c -o server.o server.c
gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -c -o common.o common.c
gcc -o server server.o common.o
</pre>

In den ersten zwei Zeilen ist der (subtile) Unterschied das Flag **-c**, welches _kompilieren aber nicht linken_ heißt. Damit ist es vor allem nunmehr nötig, die Datei _common.c_ nur einmal zu kompilieren und dann einfach in die zwei Binaries einzubinden.

Eine entsprechend angepasste Makefile sieht wie folgt aus:

<pre>
all: client server

client: client.o common.o
    gcc -o client client.o common.o

server: server.o common.o
    gcc -o server server.o common.o

client.o: client.c
    gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -c -o client.o client.c

common.o: common.c
    gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -c -o common.o common.c

server.o: server.c
    gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -c -o server.o server.c

clean:
    rm -f client server
    rm -f client.o server.o common.o
</pre>

Der Anblick dieser Makefile ist grauenvoll. Auch hier ist Copy-Paste unerwünscht, und zum Glück gibt es auch hier Abhilfe. So lässt sich eigentlich sagen, dass jede Objektdatei von einer gleich benannten Quellendatei abhängt, und die Umwandlung geschieht, den Dateinamen ausgenommen, nach dem selben Schema. <tt>make</tt> bietet hierzu sinnvolle Platzhalter:

<pre>
%.o: %.c
    gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -c -o $@ $^
</pre>

Diese Regel trifft nun auf alle Objektdateien zu, für die eine gleich benannte Quellendatei existiert (oder durch eine andere Regel erstellt werden kann) und für die es keine explizite Regel gibt. Der Platzhalter <tt>$@</tt> steht für _alle Ziele_, der Platzhalter <tt>$^</tt> für _alle Quellen_ (wie sie oben im Regelkopf angeführt sind). Die $-Platzhalter lassen sich auch in „normalen“ Regeln anführen; unsere Makefile sieht jetzt also wie folgt aus:

<pre>
all: client server

client: client.o common.o
    gcc -o $@ $^

server: server.o common.o
    gcc -o $@ $^

%.o: %.c
    gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -c -o $@ $^

clean:
    rm -f client server
    rm -f client.o server.o common.o
</pre>

## Doxygen

Oft ist die einzige verfügbare Dokumentation aus dem Kontext gerissen, sodass ein mühsames Suchspiel notwendig ist, um zu jedem Stück Dokumentation das passende Stück Code zu finden und umgekehrt.

In der Java-Welt hat das Tool _Javadoc_ dieses Problem größtenteils eliminiert: Dokumentation ist da, wo sie hingehört, und kann im Laufe des Programmierens ähnlich wie der Programmcode angepasst werden. Ein Javadoc-ähnliches Tool gibt es auch für C und andere Programmiersprachen: [_Doxygen_](http://www.stack.nl/~dimitri/doxygen/).

Bevor Doxygen ausgeführt werden kann, braucht es eine Konfigurationsdatei. In Anlehnung an <tt>make</tt>s Makefile heißt Doxygens Konfigurationsdatei _Doxyfile_ und ist ebenfalls eine Textdatei. Eine Vorlage dafür kann mit dem Befehl <tt>doxygen -g Doxyfile</tt> generiert werden, danach sind jedoch drei Anpassungen nötig:

1. Die Variable <tt>INPUT</tt> muss auf den/die Ordner, die den Quellcode enthalten, eingestellt werden. Wenn sich die Doxyfile im selben Ordner wie der Quellcode befindet, kann der aktuelle Ordner mit dem Wert <tt>.</tt> (Punkt) angedeutet werden.
2. Die Variable <tt>FILE_PATTERNS</tt> enthält die Namensmuster der Dateien, die von Doxygen verarbeitet werden sollen. Für die Entwicklung mit C ist <tt>*.c *.h</tt> optimal.
3. Die Variable <tt>EXTRACT_STATIC</tt> bestimmt, ob die Dokumentation auch statische Variablen und Funktionen enthalten soll. In dieser Übung ist der Wert <tt>YES</tt> empfohlen, da die meisten Funktionen/Variablen static sein werden.

Natürlich können auch andere Änderungen vorgenommen werden; diese sprengen jedoch den Rahmen des Einführungstutorials.

Auch das Format der Kommentare mutet eine Verwandtschaft mit Javadoc an: mehrzeilige C-Kommentare mit zwei statt einem Sternchen am Anfang.

<pre>
#include <stdio.h>

/*
 * This is not a Doxygen comment, but the next one is.
 */

/**
 * The main entry point of the program.
 *
 * @param argc The number of command-line parameters in argv.
 * @param argv The array of command-line parameters, argc elements long.
 * @return The exit code of the program. 0 on success, non-zero on failure.
 */
int main(int argc, char **argv)
{
    printf("Hello, world!\n");
    return 0;
}
</pre>

Wenn wir nun <tt>doxygen</tt> ausführen, erstellt es zwei Unterordner: <tt>html</tt> und <tt>latex</tt>. Ersterer enthält <abbr title="HyperText Markup Language">HTML</abbr>-Dateien, die im Webbrowser betrachtet werden können; letzterer [LaTeX](http://en.wikipedia.org/wiki/LaTeX)-Quelldateien. Wenn wir nun im <tt>html</tt>-Unterordner die Datei <tt>index.html</tt> öffnen, sehen wir... nichts.

Doxygen verarbeitet eine Datei nur, wenn sie am Anfang ein Kommentar mit dem <tt>@file</tt>-Tag enthält. Wenn unsere Quellendatei also <tt>cexample.c</tt> heißt, müssen wir sie wie folgt erweitern:

<pre>
/**
 * @file cexample.c
 */

#include <stdio.h>

/*
 * This is not a Doxygen comment, but the next one is.
 */

/**
 * The main entry point of the program.
 *
 * @param argc The number of command-line parameters in argv.
 * @param argv The array of command-line parameters, argc elements long.
 * @return The exit code of the program. 0 on success, non-zero on failure.
 */
int main(int argc, char **argv)
{
    printf("Hello, world!\n");
    return 0;
}
</pre>

Und wenn wir schon dabei sind, können wir auch Autor und Datum hinzufügen. (Wenn wir unseren Code in einem <abbr title="Software Configuration Management">SCM</abbr>-System speichern, ist das in vielen Fällen nicht unbedingt sinnvoll, aber da die _Betriebssysteme_-Abgaben nicht über ein SCM stattfinden, machen wir es an dieser Stelle.)

<pre>
/**
 * @file cexample.c
 * @author Ondřej Hošek (0925631) <ondrej.hosek@tuwien.ac.at>
 * @date 2012-03-15
 */

#include <stdio.h>

/*
 * This is not a Doxygen comment, but the next one is.
 */

/**
 * The main entry point of the program.
 *
 * @param argc The number of command-line parameters in argv.
 * @param argv The array of command-line parameters, argc elements long.
 * @return The exit code of the program. 0 on success, non-zero on failure.
 */
int main(int argc, char **argv)
{
    printf("Hello, world!\n");
    return 0;
}
</pre>

Beim nächsten Ausführen von Doxygen sehen wir zwar wieder eine leere Seite, doch nun ist oben eine Registerkarte _Files_ dazugekommen, wo wir dann auch zwischen den einzelnen Quellcode-Dateien wählen und ihre Dokumentation betrachten können.

## Hinweise zur Lehrveranstaltung

Nachfolgend einige nützliche Tipps zum Bestreiten der Betriebssysteme-Übung:

* Das erste Beispiel besteht aus zwei Teilen. Teil 1A ist eine Einführung in das Programmieren mit C; Teil 1B in die Netzwerkprogrammierung mit Sockets.
* Werfen Sie öfters einen Blick auf die [LVA-Homepage](http://ti.tuwien.ac.at/rts/teaching/courses/osue/), insbesondere den Abschnitt _Aktuelles_!
* Nutzen Sie das Betreuungsangebot. Annähernd jede Woche sind zu [bestimmten Uhrzeiten](http://ti.tuwien.ac.at/rts/teaching/courses/osue/laborteil#Betreute) Tutoren im Labor anwesend, die Ihnen beim Lösen der Übungen und Verstehen des Stoffs behilflich sein können.
* Beachten Sie vor der Abgabe eines Übungsbeispiels die [Richtlinien](http://ti.tuwien.ac.at/rts/teaching/courses/osue/richtlinien). Sie können sich damit manch einen Punkteabzug ersparen.
* Ein [Übungstest](http://ti.tuwien.ac.at/cps/teaching/courses/osue/downloads) steht auf der OSUE-Website zur Verfügung.