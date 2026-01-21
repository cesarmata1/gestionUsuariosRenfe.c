// César Mata Rodríguez

#include <stdio.h>
#include <string.h>

struct datosRegistro {
    char login[30];
    char password[20];
    int tipo;
};

void agregar();
void visualizar();
void incrementar();

int main() {
    int opcion;

    do {
        printf("\nElije una opcion:\n");
        printf("1) Añadir a la web\n");
        printf("2) Incrementar tipo de un user\n");
        printf("3) Visualizar usuarios\n");
        printf("4) Obtener importe total\n");
        printf("5) Huelga\n");
        printf("6) Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: agregar(); break;
            case 2: incrementar(); break;
            case 3: visualizar(); break;
            case 6: printf("...Saliendo...\n"); break;
            default: printf("...Opcion incorrecta...\n");
        }
    } while (opcion != 6);

    return 0;
}

/* ================== AÑADIR USUARIO ================== */
void agregar() {
    struct datosRegistro actual, nuevo;
    char login[30];
    FILE *f = fopen("usuariosRenfe", "rb");

    if (f == NULL) {
        f = fopen("usuariosRenfe", "wb");
        fclose(f);
        f = fopen("usuariosRenfe", "rb");
    }

    printf("Introduce el login: ");
    scanf(" %29[^\n]", login);

    while (fread(&actual, sizeof(struct datosRegistro), 1, f)) {
        if (strcmp(login, actual.login) == 0) {
            printf("Usuario ya registrado\n");
            fclose(f);
            return;
        }
    }

    strcpy(nuevo.login, login);
    printf("Introduce la password: ");
    scanf(" %19[^\n]", nuevo.password);
    nuevo.tipo = 0;

    FILE *aux = fopen("aux.bin", "wb");
    rewind(f);

    int insertado = 0;
    while (fread(&actual, sizeof(struct datosRegistro), 1, f)) {
        if (!insertado && strcmp(nuevo.login, actual.login) < 0) {
            fwrite(&nuevo, sizeof(struct datosRegistro), 1, aux);
            insertado = 1;
        }
        fwrite(&actual, sizeof(struct datosRegistro), 1, aux);
    }

    if (!insertado) {
        fwrite(&nuevo, sizeof(struct datosRegistro), 1, aux);
    }

    fclose(f);
    fclose(aux);

    remove("usuariosRenfe");
    rename("aux.bin", "usuariosRenfe");

    printf("Usuario añadido correctamente\n");
}

/* ================== VISUALIZAR USUARIOS ================== */
void visualizar() {
    FILE *f;
    struct datosRegistro reg;

    f = fopen("usuariosRenfe", "rb");
    if (f == NULL) {
        printf("No hay usuarios\n");
        return;
    }

    while (fread(&reg, sizeof(struct datosRegistro), 1, f)) {
        printf("\nLogin: %s", reg.login);
        printf("\nTipo: %d", reg.tipo);
        printf("\nPassword: %s\n", reg.password);
    }

    fclose(f);
}

/* ================== INCREMENTAR TIPO ================== */
void incrementar() {
    FILE *f;
    struct datosRegistro reg;
    char login[30];
    int pos, encontrado = 0;

    printf("Introduce el login: ");
    scanf(" %29[^\n]", login);

    f = fopen("usuariosRenfe", "rb+");
    if (f == NULL) {
        printf("No existe el fichero\n");
        return;
    }

    while (fread(&reg, sizeof(struct datosRegistro), 1, f)) {
        if (strcmp(login, reg.login) == 0) {
            encontrado = 1;

            if (reg.tipo >= 3) {
                printf("Tipo maximo alcanzado\n");
                break;
            }

            reg.tipo++;
            pos = ftell(f) - sizeof(struct datosRegistro);
            fseek(f, pos, SEEK_SET);
            fwrite(&reg, sizeof(struct datosRegistro), 1, f);

            printf("Tipo incrementado correctamente\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Usuario no encontrado\n");
    }

    fclose(f);
}



