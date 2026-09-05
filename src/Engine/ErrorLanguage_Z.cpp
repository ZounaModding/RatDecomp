#include "ErrorLanguage_Z.h"

const Char* ErrorTabLang[LANG_COUNT][type_error_count];

void InitTabError() {
    const Char** l_Tab = ErrorTabLang[0];
    for (S32 i = 0; i < LANG_COUNT; i++) {
        for (S32 j = 0; j < type_error_count; j++) {
            ErrorTabLang[i][j] = NULL;
        }
    }

    ErrorTabLang[LANG_ENGLISH_Z][error_tray] = "The Disc Cover is open.~If you want to continue the game,~please close the Disc Cover.";
    ErrorTabLang[LANG_ENGLISH_Z][error_retry_occur] = "The Game Disc could not be read.~Please read the Nintendo GameCube~Instruction Booklet for more~information.";
    ErrorTabLang[LANG_ENGLISH_Z][error_fatal] = "An error has occurred.~Turn the power off and refer to the~Nintendo GameCube Instruction Booklet~for further instructions.";
    ErrorTabLang[LANG_ENGLISH_Z][error_no_disk] = "Please insert~the %s Game Disc.";
    ErrorTabLang[LANG_ENGLISH_Z][error_wrong_disk] = "Please insert~the %s Game Disc.";

    ErrorTabLang[LANG_FRENCH_Z][error_tray] = "Le couvercle est ouvert.~Pour continuer \340 jouer,~veuillez fermer le couvercle.";
    ErrorTabLang[LANG_FRENCH_Z][error_retry_occur] = "La lecture du disque a \351chou\351.~Veuillez vous r\351f\351rer au manuel~d'instructions Nintendo GameCube~pour de plus amples informations.";
    ErrorTabLang[LANG_FRENCH_Z][error_fatal] = "Une erreur est survenue.~Eteignez la console et r\351f\351rez-vous~au manuel~d'instructions Nintendo GameCube~pour de plus amples informations.";

    ErrorTabLang[LANG_SPANISH_Z][error_tray] = "La tapa est\341 abierta.~Si quieres seguir jugando,~debes cerrar la tapa.";
    ErrorTabLang[LANG_SPANISH_Z][error_retry_occur] = "No se puede leer el disco.~Consulta el manual de instrucciones~de Nintendo GameCube para obtener~m\341s informaci\363n.";
    ErrorTabLang[LANG_SPANISH_Z][error_fatal] = "Se ha producido un error.~Apaga la consola y consulta~el manual de instrucciones de~Nintendo GameCube para obtener~m\341s informaci\363n.";
    ErrorTabLang[LANG_SPANISH_Z][error_no_disk] = "Coloca el disco de~%s.";
    ErrorTabLang[LANG_SPANISH_Z][error_wrong_disk] = "Coloca el disco de~%s.";

    ErrorTabLang[LANG_GERMAN_Z][error_tray] = "Der Disc-Deckel ist ge\366ffnet.~Bitte den Disc-Deckel schlie\337en,~um mit dem Spiel fortzufahren.";
    ErrorTabLang[LANG_GERMAN_Z][error_retry_occur] = "Diese Game Disc kann nicht~gelesen werden. Bitte lesen Sie~die Bedienungsanleitung~des Nintendo GameCube,~um weitere Informationen~zu erhalten.";
    ErrorTabLang[LANG_GERMAN_Z][error_fatal] = "Ein Fehler ist aufgetreten.~Bitte schalten Sie den~Nintendo GameCube aus und lesen~Sie die Bedienungsanleitung, um~weitere Informationen zu erhalten.";
    ErrorTabLang[LANG_GERMAN_Z][error_no_disk] = "Bitte legen Sie die~%s-Disc ein.";
    ErrorTabLang[LANG_GERMAN_Z][error_wrong_disk] = "Bitte legen Sie die~%s-Disc ein.";

    ErrorTabLang[LANG_ITALIAN_Z][error_tray] = "Il coperchio del disco \350 aperto.~Se vuoi proseguire nel gioco,~chiudi il coperchio del disco.";
    ErrorTabLang[LANG_ITALIAN_Z][error_retry_occur] = "Impossibile leggere il disco di gioco.~Per ulteriori indicazioni consulta il manuale~di istruzioni del Nintendo GameCube.";
    ErrorTabLang[LANG_ITALIAN_Z][error_fatal] = "Si \350 verificato un errore.~Spegni e consulta il manuale di~istruzioni del Nintendo GameCube~per ulteriori indicazioni.";
    ErrorTabLang[LANG_ITALIAN_Z][error_no_disk] = "Inserisci il disco di gioco~%s.";
    ErrorTabLang[LANG_ITALIAN_Z][error_wrong_disk] = "Inserisci il disco di gioco~%s.";

    ErrorTabLang[LANG_DUTCH_Z][error_tray] = "De disc-deksel is open.~Sluit de disc-deksel ~als je verder wilt spelen.";
    ErrorTabLang[LANG_DUTCH_Z][error_retry_occur] = "De spel-disc kon niet worden gelezen.~Lees de handleiding van de Nintendo GameCube~voor meer informatie.";
    ErrorTabLang[LANG_DUTCH_Z][error_fatal] = "Er is een fout opgetreden.~Zet de Nintendo GameCube uit en raadpleeg~de handleiding van de Nintendo GameCube~voor nadere instructies.";
    ErrorTabLang[LANG_DUTCH_Z][error_no_disk] = "Plaats de spel-disc met~%s.";
    ErrorTabLang[LANG_DUTCH_Z][error_wrong_disk] = "Plaats de spel-disc met~%s.";

    ErrorTabLang[LANG_JAPANESE_Z][error_tray] = "";
    ErrorTabLang[LANG_JAPANESE_Z][error_retry_occur] = "";
    ErrorTabLang[LANG_JAPANESE_Z][error_fatal] = "";
    ErrorTabLang[LANG_JAPANESE_Z][error_no_disk] = "";
    ErrorTabLang[LANG_JAPANESE_Z][error_wrong_disk] = "";
}

const Char* GetStringTabError(LanguageEnum_Z i_Enum, SystemHdlError i_Error) {
    return NULL;
}
