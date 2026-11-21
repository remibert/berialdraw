# Spécification — widget calendrier

**Nom proposé dans l’API** : `Calendar`  
**Statut** : Brouillon pour revue  
**Date** : 2026-11-19

## Périmètre

Créer un widget permettant d’afficher et de sélectionner une date. Il propose trois combos de sélection — jour, mois et année — ainsi qu’un calendrier mensuel cliquable.

Le périmètre couvre la sélection d’une date, la navigation de mois, l’affichage des jours des mois voisins, les événements de changement de date et le style du widget. Les plages de dates, les numéros de semaine et les rendez-vous ne font pas partie de la première version.

Le dépôt possède déjà les widgets [`Combo`](include/widget/combo.hpp) et [`TableView`](include/widget/table_view.hpp), mais aucune abstraction de date ou aucun widget calendrier n’a été identifié. Les combos peuvent être liés aux événements existants; le calendrier devra exposer un événement contenant la date sélectionnée.

## Scénarios utilisateur et tests

### Scénario 1 — Consulter une date (Priorité : P1)

En tant qu’utilisateur, je veux ouvrir un calendrier déjà positionné sur une date, afin de la consulter ou de la modifier.

**Test indépendant** : créer le widget avec et sans date initiale, puis vérifier la date sélectionnée et le mois affiché.

**Critères d’acceptation**

1. **Étant donné** une date initiale valide, **quand** le calendrier est créé avec cette date, **alors** cette date est sélectionnée et son mois est affiché.
2. **Étant donné** qu’aucune date initiale n’est fournie, **quand** le calendrier est créé, **alors** la date locale du jour est sélectionnée.
3. **Étant donné** que le jour sélectionné est affiché, **alors** sa cellule utilise la couleur de sélection.

### Scénario 2 — Sélectionner une date dans la grille (Priorité : P1)

En tant qu’utilisateur, je veux cliquer sur un jour de la grille, afin de sélectionner cette date.

**Test indépendant** : cliquer sur un jour du mois affiché, puis sur un jour grisé d’un mois voisin.

**Critères d’acceptation**

1. **Étant donné** un jour du mois affiché, **quand** je clique dessus, **alors** il devient la date sélectionnée et les combos sont mis à jour.
2. **Étant donné** un jour grisé appartenant au mois précédent ou suivant, **quand** je clique dessus, **alors** ce jour devient la date sélectionnée et son mois devient le mois affiché.
3. **Étant donné** que la date sélectionnée change, **alors** le widget émet un événement avec la nouvelle date.

### Scénario 3 — Choisir la date avec les combos (Priorité : P1)

En tant qu’utilisateur, je veux régler le jour, le mois et l’année à l’aide des combos, afin de saisir une date sans parcourir la grille.

**Test indépendant** : sélectionner des valeurs dans les trois combos et vérifier la date résultante, y compris pour février et une année bissextile.

**Critères d’acceptation**

1. **Étant donné** un mois et une année affichés, **quand** je choisis un jour valide, **alors** la date sélectionnée correspond aux trois valeurs.
2. **Étant donné** une date au 31 d’un mois, **quand** je passe à un mois plus court, **alors** le jour est ramené au dernier jour valide de ce mois.
3. **Étant donné** le 29 février d’une année bissextile, **quand** je passe à une année non bissextile, **alors** la date devient le 28 février.

### Scénario 4 — Parcourir les mois (Priorité : P2)

En tant qu’utilisateur, je veux aller au mois précédent ou suivant, afin de sélectionner une date proche rapidement.

**Test indépendant** : utiliser les commandes précédent/suivant depuis un mois ordinaire, puis depuis décembre et janvier.

**Critères d’acceptation**

1. **Quand** j’active « Mois précédent » ou « Mois suivant », **alors** le mois et l’année affichés avancent ou reculent correctement.
2. **Étant donné** une date sélectionnée dont le jour n’existe pas dans le mois cible, **alors** le jour est ramené au dernier jour valide.
3. **Quand** j’active « Aujourd’hui », **alors** la date du jour devient la date sélectionnée et son mois est affiché.

## Cas limites

- Février des années bissextiles et non bissextiles; mois de 28, 29, 30 et 31 jours.
- Passage de décembre à janvier et de janvier à décembre.
- Date initiale invalide ou date contenant un jour inexistant : le widget ne doit pas accepter une date invalide ni afficher une sélection incohérente.
- Saisie d’une année vide, non numérique ou hors plage prise en charge : conserver la dernière date valide et signaler que la saisie est invalide.
- Le mois commence un lundi : la semaine complète qui le précède doit tout de même être affichée.
- Le calendrier doit afficher sept lignes de semaines si six ne suffisent pas à présenter le mois et les jours de fin du mois suivant, en plus de la ligne grise obligatoire du mois précédent.
- Une horloge système indisponible ou non réglée, notamment sur une cible embarquée, ne doit pas être présentée comme une date du jour fiable.
- Les dates sont limitées aux années grégoriennes 1 à 9999; les cellules adjacentes qui dépasseraient cette plage sont vides et non sélectionnables.

## Exigences

### Exigences fonctionnelles

- **REQ-001** : Le widget DOIT accepter une date initiale valide; si elle n’est pas fournie, il DOIT sélectionner la date locale du jour.
- **REQ-002** : Le widget DOIT présenter, sur sa rangée supérieure, les combos dans cet ordre : jour à gauche, mois au centre, année à droite.
- **REQ-003** : Le combo du jour DOIT proposer uniquement les jours valides du mois et de l’année sélectionnés.
- **REQ-004** : Le combo du mois DOIT permettre de choisir les douze mois; le combo de l’année DOIT permettre de saisir une année et de choisir parmi les valeurs proposées.
- **REQ-005** : Toute modification valide d’un combo DOIT produire une date valide et actualiser la grille ainsi que les autres combos.
- **REQ-006** : La grille DOIT avoir sept colonnes ordonnées du lundi au dimanche, avec les noms des jours en en-tête.
- **REQ-007** : La grille DOIT afficher au moins six lignes de semaines et DOIT placer en première ligne une semaine complète précédant le mois affiché, y compris lorsque le mois commence un lundi. Elle DOIT afficher une septième ligne lorsque nécessaire pour couvrir tout le mois et sa fin.
- **REQ-008** : Les jours hors du mois affiché DOIVENT être visuellement grisés. Ils DOIVENT rester cliquables; un clic sélectionne la date correspondante et bascule l’affichage sur son mois.
- **REQ-009** : Un clic sur un jour du mois affiché DOIT sélectionner cette date, actualiser les combos et déclencher l’événement de changement de date.
- **REQ-010** : La date sélectionnée DOIT se distinguer avec une couleur de sélection. Si elle diffère de la date du jour, cette dernière peut recevoir un marqueur visuel distinct.
- **REQ-011** : Le widget DOIT fournir des commandes « Mois précédent » et « Mois suivant ». Elles changent le mois affiché et déplacent la date sélectionnée d’un mois, en conservant le numéro du jour lorsque possible et en le ramenant au dernier jour du mois sinon.
- **REQ-012** : Le widget DOIT fournir une commande « Aujourd’hui » qui sélectionne la date locale du jour et affiche son mois.
- **REQ-013** : Les couleurs et les épaisseurs de trait propres au calendrier DOIVENT être configurées dans `calendar.sty`, et non codées en dur.
- **REQ-014** : Le widget DOIT refuser les dates invalides et les années saisies invalides sans remplacer la dernière date valide.
- **REQ-015** : Une modification effective de la date sélectionnée, qu’elle provienne d’un clic, d’un combo, d’une commande de navigation ou de l’API, DOIT émettre un événement contenant la date résultante. Une opération qui ne change pas la date NE DOIT PAS émettre de changement de date.
- **REQ-016** : L’obtention de la date locale courante DOIT passer par une fonction bas niveau dont l’implémentation est adaptée à la plateforme; aucun appel système propre à un OS ne DOIT se trouver dans `Calendar`.
- **REQ-017** : Une façade de date de plus haut niveau DOIT isoler `Calendar` de l’interface bas niveau et signaler explicitement l’échec si la date courante n’est pas disponible.

### Entités principales

- **`CalendarDate`** : date civile grégorienne composée d’une année, d’un mois et d’un jour; fournit une validation et une manière d’obtenir la date locale courante.
- **Date sélectionnée** : valeur courante exposée à l’application; elle est toujours valide.
- **Mois affiché** : période représentée par la grille et les combos; il correspond au mois de la date sélectionnée.
- **Jour de cellule** : date complète représentée par une cellule, y compris lorsqu’elle appartient au mois précédent ou suivant.
- **Événement de changement de date** : événement contenant la nouvelle `CalendarDate` et émis lorsqu’elle change.

## Proposition d’API C++

Les noms d’API et de code sont en anglais, conformément aux conventions du dépôt.

```cpp
struct CalendarDate
{
	int year;
	uint8_t month;
	uint8_t day;

	bool is_valid() const;
};

class Date
{
public:
	static bool today(CalendarDate & result);
};

class Calendar : public Widget, public CalendarStyle
{
public:
	Calendar(Widget * parent);
	Calendar(Widget * parent, const CalendarDate & date);

	const CalendarDate & date() const;
	bool date(const CalendarDate & value);

	int year() const;
	bool year(int value);
	uint8_t month() const;
	bool month(uint8_t value);
	uint8_t day() const;
	bool day(uint8_t value);

	void previous_month();
	void next_month();
	void today();

	void serialize(JsonIterator & it) override;
	void unserialize(JsonIterator & it) override;
	StyleCascadeMode style_cascade_mode() const override;
};
```

Le calendrier posséderait ses contrôles internes : les trois combos, les deux commandes de navigation, la commande « Aujourd’hui » et la grille de dates basée sur `TableView`. Le code appelant pourrait écouter l’événement à l’aide du mécanisme `bind` existant :

```cpp
void on_date_changed(Widget * widget, const CalendarEvent & event);

calendar->bind(this, &Page::on_date_changed);
```

`CalendarEvent` expose la nouvelle date. Les setters `date`, `year`, `month` et `day` retournent `false` si l’entrée est invalide, sans modifier la date courante. Une modification du mois ou de l’année qui rend le jour courant impossible le ramène au dernier jour valide.

## Accès à la date courante et séparation par plateforme

La date du jour doit être obtenue via une couche système, et non calculée ou interrogée directement dans le widget. Dans l’architecture actuelle, [`bd_system.h`](include/tool/bd_system.h) déclare les fonctions bas niveau, [`bd_system.cpp`](src/tool/bd_system.cpp) fournit des wrappers, et les fonctions dépendantes de la plateforme sont séparées, notamment dans [`bd_system_win32.cpp`](src/tool/bd_system_win32.cpp) et [`bd_system_posix.cpp`](src/tool/bd_system_posix.cpp). Aucune fonction de récupération de la date n’est actuellement déclarée dans `bd_system.h`.

Proposition de chemin d’accès :

1. Ajouter dans `bd_system.h` une fonction C bas niveau, par exemple `bd_get_local_date(...)`, qui fournit l’année, le mois et le jour locaux et retourne un statut de réussite. Implémenter cette fonction dans le fichier système approprié pour chaque plateforme prise en charge; ajouter une implémentation ESP32 si nécessaire.
2. Garder `CalendarDate` comme valeur civile indépendante du système d’exploitation.
3. Ajouter une façade de plus haut niveau, proposée sous le nom `Date`, avec `Date::today(CalendarDate & result)`. Elle appelle la couche bas niveau, valide le résultat et garde `Calendar` indépendant des API OS.

Cette séparation reprend le principe de [`File`](include/tool/file.hpp) : une API de plus haut niveau masque les détails des implémentations sous-jacentes. Si plusieurs fournisseurs de date doivent être interchangeables (horloge système, RTC, horloge simulée ou date fournie par l’application), une interface dédiée analogue à [`FileInterface`](include/tool/file_interface.hpp) pourra être ajoutée; elle n’est pas indispensable si le wrapper `bd_system` suffit.

Si l’horloge de la plateforme n’est pas initialisée ou ne fournit pas de date, `Date::today` DOIT indiquer l’échec. Le calendrier reste sans date sélectionnée, signale l’erreur et permet à l’application de fournir ensuite une date valide par l’API; il ne doit ni inventer une date de repli ni afficher une valeur invalide.

## Style

Prévoir un fichier `calendar.sty` pour chaque thème, par exemple `resources/styles/pearl/calendar.sty`. Il contient toutes les couleurs et épaisseurs de trait spécifiques au calendrier, y compris celles nécessaires aux cellules, à l’en-tête des jours, à la sélection, aux dates hors mois, aux week-ends et à l’indication de la date du jour.

Clés de style proposées :

| Clé | Usage |
|---|---|

| `weekday_text_color` | Couleur du texte du jour de la semaine |
| `weekday_font_size` | Taille de police des noms des jours de la semaine |
| `weekday_back_color` | Fond des cellules de l’en-tête des jours de la semaine |

| `outside_day_text_color` | Texte des jours hors du mois affiché |
| `outside_day_back_color` | Fond limité aux cellules des jours hors du mois affiché |
| `day_text_color` | Texte des jours du mois affiché |
| `day_font_size` | Taille de police des numéros des jours |
| `day_back_color` | Fond limité aux cellules des jours du mois affiché |

| `day_selected_text_color` | Texte de la date sélectionnée |
| `day_selected_back_color` | Fond de la date sélectionnée |
| `grid_color` | Couleur du quadrillage |
| `grid_thickness` | Épaisseur des traits du quadrillage |
| `day_border_color` | Couleur des bordures des cellules de jour |
| `day_border_thickness` | Épaisseur des bordures des cellules de jour |

`calendar.sty` configure uniquement la grille, les jours et leurs bordures. Les combos et boutons de navigation conservent leurs styles habituels (`combo.sty`, `combo_button.sty` et `button.sty`).

## Critères de réussite mesurables

- **SC-001** : Pour toute date valide fournie, le widget affiche cette date dans les combos et sélectionne sa cellule.
- **SC-002** : Pour chacun des douze mois et pour les cas bissextiles, la grille présente les dates dans l’ordre exact et ne propose aucune date impossible.
- **SC-003** : Chaque mois affiché présente sept colonnes du lundi au dimanche et au moins six lignes de semaines. Si six lignes laissent plus d’une semaine après le mois, la grille ajoute une semaine antérieure afin de conserver les dates du mois suivant accessibles sans afficher deux semaines complètes en fin de grille.
- **SC-004** : Chaque voie de sélection — clic sur une cellule, combo, navigation — aboutit à la même date dans le widget et dans l’événement émis.
- **SC-005** : Après toute sélection, l’utilisateur peut identifier sans ambiguïté la date sélectionnée grâce à son style.
- **SC-006** : Toutes les couleurs et épaisseurs de trait spécifiques au calendrier peuvent être modifiées dans le fichier de style sans modifier le code du widget.

## Hypothèses et décisions à confirmer

- Le calendrier suit le calendrier grégorien et utilise la date locale du système pour « aujourd’hui ».
- Les années proposées dans le combo sont éditables; les suggestions de la liste ne limitent pas les dates accessibles. Les années prises en charge vont de 1 à 9999 et la liste suggère une fenêtre de dix ans de part et d’autre de l’année sélectionnée, limitée à cette plage.
- Il n’existe pas encore de type date commun repéré dans le dépôt. `CalendarDate` est donc un type public proposé, à réévaluer si un type date partagé est ajouté.
- La source de la date civile locale doit être implémentée pour chaque plateforme. Les cibles sans horloge réglée devront recevoir une date fiable de l’application ou d’une source système initialisée.
- Les textes, noms des mois et noms des jours du calendrier utilisent le composant de traduction décrit dans [la spécification des catalogues de langue](specification-langues.md). Les règles de localisation des formats de date restent hors périmètre du widget.

## Suggestions hors périmètre initial

- Ajouter des dates minimale et maximale configurables pour interdire la sélection hors d’une période.
- Ajouter éventuellement les numéros de semaine dans une colonne dédiée; cela porterait la grille à huit colonnes et doit rester désactivable.
- Définir les interactions clavier (flèches pour changer de jour ou de semaine, raccourcis pour changer de mois) en complément de la sélection à la souris.
- Confirmer si le marqueur distinct de la date du jour est requis lorsque la date sélectionnée n’est pas aujourd’hui.

## Maquette visuelle

La maquette illustre novembre 2026 : les commandes, les trois combos et « Aujourd’hui » partagent une seule rangée sans titre ni libellés de champs. La grille commence par les jours d’octobre précédant le mois, comprend six lignes de semaines et le 19 novembre est sélectionné. Les couleurs sont indicatives; la version finale doit suivre `calendar.sty`.

![Maquette du widget calendrier](maquette-calendrier.svg)

## Référence de conception

La documentation de Qt `QCalendarWidget` expose notamment le premier jour de semaine, la date sélectionnée, les commandes mois précédent/suivant et l’action « aujourd’hui ». Ces commandes sont recommandées ici en complément des combos pour accélérer la navigation : [QCalendarWidget — documentation Qt 6](https://doc.qt.io/qt-6/qcalendarwidget.html).
