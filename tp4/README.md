# TP4  Codes convolutionnels
# Binôme : ABBOUD Oussama - OUMLIL Abdellah
--------------------------------------------

## Structure du code
-------------------------------
- Fichier **utils.hpp**:
    - Contient les fonctions necessaires pour le codage et le decodage

```cpp
    inline vector<bitset<K>> GSM_decode(vector<bitset<N>> mess_tra)
    /*
        L'algorithme de Viterbi a été utilisé pour identifier la séquence de code ayant la plus forte probabilité de correspondre au message transmis, à partir du message reçu. 
    
        Des vecteursont été créés pour représenter les chemins potentiels. Pour chaque mot de N bits du message transmis, deux nouveaux chemins ont été créés en utilisant le calcul de la distance de Hamming. 
        Ces chemins ont été ajoutés à une liste globale qui représente un treillis de codage, et la séquence de bits la moins éloignée de l'origine a été sélectionnée en isolant le chemin ayant la plus petite distance de Hamming cumulée entre tous ces liens.
    */
```

- Résultats:
    - voici le résultat du décodage d'un message auquel nous avons appliqué des perturbations pour simuler le bruit d'un canal de transmission. le principe est de choisir aléatoirement une position dans notre bitset et l'inverser. On pourra définir le nombre d'erreurs au début du programme.
```cpp
Source Message   :  0 1 0 0 0 1 0 1 1 1 1 0 0 0 0 0
Coded Message    :  00 11 10 00 11 00 10 11 10 10 10 10 00 00 11 00
Received Message :  00 11 10 00 01 00 10 11 10 11 10 10 00 00 11 00
Decoded Message  :  0 1 0 0 0 1 0 1 1 1 1 0 0 0 0 0
```