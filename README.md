# Software-Development-for-Information-Systems
Project for class of Software Development for Information Systems, DIT UOA, 2024.
Team members:
  - Dimitris Dimochronis | 1115202100034
  - Nikos Pentasilis | 1115202100152 
  - Giorgos Ragkos | 1115202100167

## Build and Run project
Η παροχή παραμέτρων γίνεται μέσω ενός configuration file, το όνομα του οποίου δίνεται ως όρισμα στην εκτέλεση του προγράμματος.
Το αρχείο πρέπει να έχει την εξής μορφή:
```
dataset=data/ANN_SIFT10K/siftsmall_base.fvecs
query=data/ANN_SIFT10K/siftsmall_query.fvecs
groundtruth=data/ANN_SIFT10K/siftsmall_groundtruth.ivecs
k=100
R=14
L=100
a=1.2
n=
d=
q_idx=
```
### Για την **εκτέλεση** του project:
  ```
  make
  ./bin/vamana config.txt
  ```
### Για την εκτέλεση των **tests**:
  ```
  make test
  ```
### Για τον **καθαρισμό** του directory:
  ```
  make clean
  ```

## Workflow
Υπάρχει workflow μέσω Github Actions ώστε κάθε commit να ελέγχεται αυτόματα για την ορθότητα του κώδικα και την επιτυχία των tests.

## Parametres and Data types
Στο αρχείο header.h υπάρχουν 2 typedef, ένα για το data type των δεδομένων και ένα για την ευκλείδια απόσταση.
Στην παρούσα φάση και μετά από δοκιμές, θεωρήσαμε ότι για το dataset ANN_SIFT10K αρκεί το float. Παρόλα αυτά, αλλάζοντας αυτά τα typedef μπορούμε να τεστάρουμε και άλλους τύπους δεδομένων.

Για τις παρακάτω παραμέτρους, οι οποίες υπάρχουν και στον κώδικα, έχουμε 100% Recall σε 3.6', στα μηχανήματα της σχολής.

Έπειτα από δοκιμές που κάναμε, για R>log(n), το Recall@k για το ANN_SIFT10K dataset κυμαίνεται μεταξύ 92% και 100%.
```
@linux15:~/Software-Development-for-Information-Systems$ ./bin/vamana config.txt
 || Dataset: data/ANN_SIFT10K/siftsmall_base.fvecs
 || Query: data/ANN_SIFT10K/siftsmall_query.fvecs
 || Groundtruth: data/ANN_SIFT10K/siftsmall_groundtruth.ivecs
 || k: 100
 || R: 14
 || L: 100
 || a: 1.2
 || Size: 10000
 || Dimension: 128
====================================================================
 || Time taken: 220.484 seconds (= 3.67473 minutes)
 || Size of neighbors found matches k.
 || Number of neighbors found in expected neighbors: 100.
 || Recall@k: 100%.
```

## Graph
- Για την αναπαράσταση του γράφου χρησιμοποιούνται vectors, για απευθείας random indexing.<br/>
- Για κάθε κόμβο, κρατάμε σε ένα struct το vector με τα δεδομένα και ένα unordered set με τους out neighbours αυτού. Η επιλογή του set 
  έγινε ώστε να αποφύγουμε τα duplicates χωρίς να ελέγχουμε χειροκίνητα σε κάθε εισαγωγή.<br/>
- Παρέχονται οι κατάλληλες συναρτήσεις δημιουργίας κόμβου, προσθήκης του σε γράφο καθώς και η εύρεση κόμβου στον γράφο με βάση το data 
  point του.<br/>
-Υπάρχει επίσης συνάρτηση εκτύπωσης των σημείων του γράφου καθώς και εκτύπωσης των out neighbours κόμβου.<br/>

## Helper Functions
- Χρησιμοποιούμε **squared euclidean distance** διότι αυτή είναι και η μετρική του dataset.
- Η **random_permutation** επιστρέφει μια τυχαία μετάθεση των αριθμών από 0 εώς n - 1.
- Η **get_data** επιστρέφει το dataset των κόμβων του δοθέντος γράφου.
- Η **random_dataset** και **random_query** δημιουργούν ένα τυχαίο dataset και query αντίστοιχα.
- Η **check_results** ελέγχει τα αποτελέσματα του greedy search. Εάν πάρει ως όρισμα το **groundtruth** κάνει το απαραίτητο cross-check, αλλιώς υπολογίζει manually τους κοντινότερους γείτονες και τους συγκρίνει με τα αποτελέσματα του αλγορίθμου.

## Vamana indexing
- Για την αρχικοποίηση του τυχαίου γράφου, επιλέγονται για κάθε κόμβο τυχαίοι R κόμβοι ώστε να προστεθούν οι κατάλληλες ακμές.
- Κρατάμε το vector N_out_j_p που αποτελεί το $N_{out}(\sigma (i)) \bigcup \{\sigma (i)\}$ και το ανανεώνουμε κατάλληλα σε κάθε 
  επανάληψη.

## main
Διαβάζει το **configuration file** και πράττει αναλόγως: αν δίνονται **n, d $\neq$ {}**, τότε χρησιμοποιούνται τα αρχεία που δίνονται από τον χρήστη. Διαφορετικά, παράγεται ένα **τυχαίο dataset** μεγέθους n, με δεδομένα διάστασης n και παράγεται και το αντίστοιχο query point. Δημιουργείται ο γράφος από το **vamana indexing** και διατρέχεται με τη **greedy search**. Στο τέλος, καλεί την **check_results** για να ελέγχξει την ορθότητα των αποτελεσμάτων. Επίσης, αν δωθεί συγκεκριμένο q_idx, τότε η greedy search τρέχει μόνο για αυτό. Διαφορετικά, τρέχει για κάθε query στο δοθέν αρχείο.

## Optimization
- Αποφεύχθηκε όσο γίνεται, η χρήση της find_data_in_graph για εύρεση δεδομένων στον γράφο καθώς απαιτεί διάσχιση όλου του γράφου.
- Χρησιμοποιήθηκαν αρκετά reserve για γρηγορότερο indexing.
- Χρησιμοποιήθηκε το flag -O3 στο compile.

# Διαμοιρασμός εργασιών μεταξύ των μελών της ομάδας
Ο κύριος διαχωρισμός έγινε ως εξής:
- Δημήτρης Δημοχρόνης: σχεδίαση γράφου και επιλογή δομών, Vamana Indexing, μερικώς optimization και λειτουργικότητα της main,
- Νίκος Πεντασίλης: Pruning, Data Reading και optimization medoid,
- Γιώργος Ράγκος: Greedy Search, Unit Testing, Data Reading.
Ο κώδικας πέρασε από όλους και όλοι κάναμε τροποποιήσεις σε κώδικα άλλου μέλους.
Έγιναν αρκετά pull requests από την ομάδα καθώς και πολλά sessions για την επίλυση προβλημάτων και την βελτίωση του κώδικα.
