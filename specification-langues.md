# Spécification — catalogues de langue BerialDraw

**Nom proposé du composant** : `Language`  
**Statut** : Brouillon pour revue  
**Date** : 2026-11-20

## Périmètre

Ajouter à BerialDraw un mécanisme léger de traduction d’identifiants de texte, inspiré de l’usage de `gettext()` et de sa convention `_()`. Le composant sélectionne un catalogue JSON selon la langue active et renvoie la traduction associée à une clé.

La première version gère uniquement la correspondance simple **identifiant → texte UTF-8**. Elle ne gère ni pluriels, ni paramètres, ni contexte de traduction, ni formats de date. Les fichiers sont chargés par le système `File` existant, pour permettre leur lecture depuis un dossier ou depuis une archive ZIP montée dans les chemins de ressources.

Le calendrier défini dans [la spécification du widget calendrier](specification-calendrier.md) constitue le premier consommateur et fournit les exemples de libellés de cette spécification.

## Scénarios utilisateur et tests

### Scénario 1 — Choisir la langue de l’interface (Priorité : P1)

En tant qu’application BerialDraw, je veux sélectionner un catalogue correspondant à la langue choisie, afin d’afficher les libellés dans cette langue.

**Test indépendant** : sélectionner successivement deux langues, puis demander la traduction d’une même clé connue dans les deux catalogues.

**Critères d’acceptation**

1. **Étant donné** un catalogue disponible `fr-FR.json`, **quand** l’application sélectionne `fr-FR`, **alors** une clé traduite renvoie sa valeur française.
2. **Étant donné** qu’aucune langue n’a été sélectionnée explicitement, **quand** le composant est initialisé, **alors** il utilise le catalogue par défaut `en-US`.
3. **Étant donné** un changement réussi de langue, **quand** une traduction est demandée ensuite, **alors** le résultat provient du catalogue nouvellement sélectionné.

### Scénario 2 — Lire les textes du calendrier (Priorité : P1)

En tant qu’utilisateur, je veux voir les titres, commandes, mois et jours du calendrier dans la langue active.

**Test indépendant** : charger le calendrier après la sélection de `fr-FR`, puis vérifier ses libellés; répéter avec `en-US`.

**Critères d’acceptation**

1. **Étant donné** que `fr-FR` est sélectionné, **quand** le calendrier est créé, **alors** son titre, ses commandes, les noms de mois et les en-têtes de jours sont français.
2. **Étant donné** que les libellés sont absents d’un catalogue régional, **quand** ils existent dans `en-US.json`, **alors** leur traduction par défaut est utilisée.
3. Les nombres des jours restent des valeurs numériques; ils ne sont pas obtenus par recherche dans le catalogue.

### Scénario 3 — Utiliser les ressources empaquetées (Priorité : P2)

En tant qu’application déployée avec ses ressources dans une archive, je veux charger les catalogues sans les extraire manuellement.

**Test indépendant** : charger un catalogue via son chemin de ressource dans une archive ZIP, puis vérifier une traduction connue.

**Critères d’acceptation**

1. **Étant donné** un catalogue JSON accessible par `File` dans un dossier ou dans un ZIP, **quand** sa langue est sélectionnée, **alors** son contenu est disponible au mécanisme de traduction.
2. **Étant donné** un fichier JSON absent ou invalide, **quand** l’application demande cette langue, **alors** l’échec est signalé et la langue/catalogue déjà actif reste inchangé.

## Cas limites

- Une clé existe dans le catalogue sélectionné, mais sa valeur est vide.
- Une clé est absente du catalogue sélectionné et du catalogue par défaut.
- Le fichier ciblé n’existe pas, n’est pas lisible ou contient un JSON invalide.
- Le champ `locale` du fichier ne correspond pas à la langue demandée.
- La langue demandée utilise un identifiant sans catalogue exact, par exemple `fr-CA` alors que seul `fr.json` est disponible.
- Une traduction contient des caractères accentués ou non latins encodés en UTF-8.
- La langue change après la création d’un widget dont les libellés sont déjà copiés dans ses propriétés; la retranscription automatique de toute l’interface n’est pas incluse dans cette première version.

## Exigences

### Exigences fonctionnelles

- **REQ-001** : Le système DOIT charger un catalogue de langue à partir d’un fichier JSON UTF-8.
- **REQ-002** : Les catalogues DOIVENT se trouver sous `resources/languages/`, dont l’accès configurable est exposé sous le chemin de ressource `$(ui.languages)`.
- **REQ-003** : Chaque catalogue DOIT être un fichier par langue/locale, nommé avec son identifiant BCP 47 canonique et l’extension `.json` (par exemple `fr-FR.json`).
- **REQ-004** : Le format JSON DOIT contenir un identifiant de locale et un objet `messages` associant des clés uniques à des valeurs textuelles.
- **REQ-005** : Le composant DOIT permettre de sélectionner explicitement une langue; en l’absence de sélection, il DOIT utiliser le catalogue par défaut `en-US.json`.
- **REQ-006** : La fonction de traduction DOIT rechercher une clé exacte dans le catalogue sélectionné et renvoyer sa traduction si elle existe.
- **REQ-007** : Si une clé manque dans le catalogue sélectionné, le système DOIT rechercher cette clé dans le catalogue par défaut `en-US.json`.
- **REQ-008** : Si une clé est absente des deux catalogues, le système DOIT renvoyer l’identifiant fourni, afin que le texte manquant soit identifiable.
- **REQ-009** : Le chargement d’une nouvelle langue DOIT être atomique : une erreur de lecture, de validation ou d’analyse JSON DOIT être signalée et ne DOIT PAS remplacer le catalogue actif.
- **REQ-010** : Le système DOIT lire les catalogues par l’intermédiaire de `File`, afin de supporter les fichiers locaux et les chemins de ressources en ZIP.
- **REQ-011** : La première version DOIT se limiter au remplacement simple d’une clé par une chaîne; elle NE DOIT PAS interpréter les pluriels, placeholders, contextes ni domaines.
- **REQ-012** : Les composants BerialDraw DOIVENT pouvoir demander les textes par l’API du composant; une forme raccourcie `_("<id>")` DOIT être proposée sans rendre obligatoire une macro globale dans tous les en-têtes publics.
- **REQ-013** : Le calendrier DOIT obtenir du catalogue ses textes d’interface, y compris les noms des mois et les libellés des jours de semaine. Les couleurs et épaisseurs de trait restent dans les fichiers de style et NE DOIVENT PAS être placées dans les catalogues de langue.
- **REQ-014** : Le changement de langue DOIT affecter toute recherche de traduction effectuée après la sélection. La mise à jour automatique de textes déjà copiés dans des widgets existants est hors périmètre de la première version.
- **REQ-015** : Une locale régionale sans fichier dédié DOIT pouvoir utiliser le catalogue de sa langue générique, puis le catalogue par défaut, avant de retourner la clé si elle est toujours absente.
- **REQ-016** : `Language` DOIT être un service singleton possédé par `UIManager`, créé lors de `UIManager::init()` et détruit lors de `UIManager::deinit()`, selon le cycle de vie des autres services gérés par `UIManager`.
- **REQ-017** : `UIManager` DOIT exposer l’instance active via `UIManager::language()`. Les applications DOIVENT sélectionner et consulter la langue via cette instance après l’initialisation de `UIManager`.
- **REQ-018** : Le chemin `$(ui.languages)` DOIT être défini par `UIManager` avant la création et le chargement du service `Language`.

### Entités principales

- **Catalogue de langue** : document JSON UTF-8 décrivant une locale et ses traductions.
- **Identifiant de message** : clé stable et unique, indépendante du texte traduit, par exemple `calendar.action.today`.
- **Langue active** : locale sélectionnée pour les recherches courantes.
- **Catalogue par défaut** : catalogue complet `en-US.json` utilisé au démarrage et comme repli.

## Proposition de composant et d’API

### Nom et espace de noms

Le composant est proposé sous le nom `Language`, dans l’espace de noms existant `berialdraw`. Aucun nouvel espace de noms imbriqué n’est nécessaire pour un service simple, et cela suit la convention actuelle où les classes publiques du framework vivent dans `berialdraw`.

Comme les autres services globaux, `Language` est détenu par `UIManager` et accessible par son accessor statique. `UIManager` possède une seule instance active; les applications ne créent ni ne détruisent directement le service.

La fonction de traduction canonique est une méthode explicite. Pour éviter qu’une macro `_` ne pollue tous les fichiers clients ou entre en collision avec GNU gettext, la syntaxe `_("<id>")` est fournie en option par `tool/language_gettext.hpp`; les en-têtes publics principaux ne définissent pas cette macro implicitement.

### API proposée

```cpp
namespace berialdraw
{
	class Language
	{
	public:
		bool select(const char * locale);
		const String & locale() const;
		String translate(const char * id) const;
	};
}
```

`UIManager::language()` retourne le pointeur vers l’instance gérée par `UIManager`. `select()` charge et valide le catalogue demandé avant de remplacer la langue active. Il retourne `false` et conserve la sélection précédente si le chargement échoue. Pour une locale telle que `fr-CA`, le composant essaie, dans l’ordre, `fr-CA.json`, `fr.json`, puis `en-US.json`.

L’appel explicite recommandé est :

```cpp
UIManager::language()->select("fr-FR");
label->text(UIManager::language()->translate("calendar.action.today"));
```

Un en-tête facultatif dédié peut offrir la forme souhaitée :

```cpp
#define _(id) ::berialdraw::UIManager::language()->translate(id)

label->text(_("calendar.action.today"));
```

La syntaxe raccourcie est utilisable après `UIManager::init()`, lorsque le service existe. Un en-tête de compatibilité facultatif peut la définir sans imposer une macro globale dans les en-têtes principaux. L’application qui utilise déjà gettext ne doit pas inclure simultanément cet en-tête, car les deux bibliothèques peuvent définir `_`.

### API Python

Le binding Python expose l’instance gérée par `UIManager`; `Language` ne se construit pas directement depuis Python. Après l’initialisation de l’interface, l’application peut sélectionner une locale et traduire un identifiant :

```python
language = UIManager.language()
language.select("fr-FR")
today_label = language.translate("calendar.action.today")
print(language.locale)
```

### Répertoire et format des catalogues

Répertoire recommandé : `resources/languages/`  
Extension recommandée : `.json`  
Noms recommandés : `en-US.json`, `fr-FR.json`; un catalogue générique peut porter le nom `fr.json`.

Lors de `UIManager::init()`, la configuration des ressources ajoute `$(ui.languages) = "$(ui.root-dir)/languages"` avant de créer le service `Language`. Puisque `File` sait résoudre les chemins locaux et `zip://`, le catalogue peut être distribué sous forme de fichier séparé ou dans l’archive de ressources existante. L’instance `Language` est libérée avec les autres services lors de `UIManager::deinit()`.

Exemple de `resources/languages/fr-FR.json` :

```json
{
  "locale": "fr-FR",
  "messages": {
    "calendar.action.today": "Aujourd’hui",
    "calendar.weekday.monday.short": "Lun",
    "calendar.weekday.tuesday.short": "Mar",
    "calendar.weekday.wednesday.short": "Mer",
    "calendar.weekday.thursday.short": "Jeu",
    "calendar.weekday.friday.short": "Ven",
    "calendar.weekday.saturday.short": "Sam",
    "calendar.weekday.sunday.short": "Dim",
    "calendar.month.january": "janvier",
    "calendar.month.february": "février",
    "calendar.month.march": "mars",
    "calendar.month.april": "avril",
    "calendar.month.may": "mai",
    "calendar.month.june": "juin",
    "calendar.month.july": "juillet",
    "calendar.month.august": "août",
    "calendar.month.september": "septembre",
    "calendar.month.october": "octobre",
    "calendar.month.november": "novembre",
    "calendar.month.december": "décembre"
  }
}
```

`resources/languages/en-US.json` contient les mêmes identifiants avec les textes anglais et constitue le catalogue complet de repli.

## Critères de réussite mesurables

- **SC-001** : Une même clé connue renvoie le texte de chacune de deux langues après sélection de leur catalogue respectif.
- **SC-002** : Toute clé absente d’un catalogue régional mais présente dans `en-US.json` renvoie le texte anglais par défaut.
- **SC-003** : Une clé inconnue dans le catalogue sélectionné et dans le catalogue par défaut renvoie exactement l’identifiant demandé.
- **SC-004** : Les catalogues JSON peuvent être chargés sans extraction préalable, depuis un chemin local comme depuis un chemin ZIP accepté par `File`.
- **SC-005** : Un fichier invalide ou inaccessible ne remplace jamais le catalogue actif et produit un échec observable.
- **SC-006** : Tous les libellés fixes visibles du calendrier et ses noms de jours/mois sont obtenus par identifiants de traduction, sans chaîne d’interface codée en dur dans le widget.

## Hypothèses et limites

- Les textes des catalogues sont encodés en UTF-8.
- `en-US.json` est le catalogue par défaut et doit fournir une entrée pour chaque identifiant pris en charge.
- `UIManager::language()` n’est disponible qu’entre l’initialisation et la désinitialisation de `UIManager`; les applications sélectionnent la langue après l’initialisation.
- Les fichiers utilisent le support JSON existant du projet; le chargeur ne définit pas un nouveau format de sérialisation.
- Le composant ne formate pas les nombres, dates ou heures selon les conventions régionales; il fournit seulement les traductions de textes.
- Les traductions déjà affectées à des propriétés de widgets ne sont pas automatiquement recalculées lors d’un changement de langue dans la première version. L’application sélectionne donc normalement la langue avant de construire l’interface.
- Les pluriels, paramètres, commentaires de traducteurs, extraction automatique des chaînes et outils de gestion des traductions ne sont pas inclus.
