* stoffumfang wie auf der HP beschrieben

* theorieteil (30 pkt) + praxisteil (50 pkt)

* am ende des praktischen teils _muessen_ die programme mit deliver abgegeben werden. man kann nicht mehr sagen man moechte lieber nicht noch einmal deliver aufrufen weil man aenderungen gemacht hat => nach jedem deliver eine sicherheitskopie anlegen, weiter arbeiten und am ende die kopie bei der man die meisten punkte bekommen hat zurueck kopieren und diese mit deliver abgeben.

<pre>
$ vim foo.c
$ deliver 1 -> 10 pkt
$ cp foo.c foo.c.10
$ vim foo.c
$ deliver 1 -> 5 pkt
....
#abgabe rueckt nahe
$ cp foo.c.10 foo.c
$ deliver 1
</pre>

git ist installiert und eignet sich hervorragend fuer die versionierung.

* der punkt exec im stoff inkludiert die gesamte exec-family (man exec). zur vorbereitung zb einfach mal eine shell mit allen unterschiedlichen execs starten.

* MC-test: jedes ding bei dem man ein kreuz machen kann gibt -1, 0, 1 fuer falsch, nichts, richtig. die summe wird dann auf die maximal 30 moeglichen punkte skaliert. im schlimmsten fall geht man mit 0 punkten vom MC-test weg.