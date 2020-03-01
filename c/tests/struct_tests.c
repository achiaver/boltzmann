#include <stdio.h>
#include <string.h>

struct person_info {
    int birth_year;
    char country[3];
    char first_name[50];
    char last_name[50];
};

void print_person(struct person_info person) {
    printf("First Name: %s\nLast Name: %s\nBirth Year: %d\nHome Country: %s\n\n", 
            person.first_name, person.last_name, person.birth_year, person.country);
}


//void change_person(struct person_info person) {
//Para poder modificar a struct tem que passar ela como referencia (ponteiro)
void change_person(struct person_info *person) {
    person->birth_year = 2987;
    strcpy(person->first_name, "Future Artur");
    strcpy(person->last_name, "Grover");
    strcpy(person->country, "BRf");

    print_person(*person);
}

int main() {
    struct person_info local_person;
    local_person.birth_year = 1987;
    strcpy(local_person.country, "BR");
    strcpy(local_person.first_name, "Artur");
    strcpy(local_person.last_name, "Grover");

    printf("---- Mostra dados da primeira pessoa ----\n");
    print_person(local_person);

    printf("---- Mostra dados depois de mudar a pessoa ----\n");
    change_person(&local_person);
    print_person(local_person);

    return 0;
}
