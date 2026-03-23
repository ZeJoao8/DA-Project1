#include "Menu.h"
#include "Assignments.h"
#include "../utils/DataLoader.h"
#include <iostream>

using namespace std;

// --- execution modes ---

/**
 * @brief Menu loop for Interactive Mode execution.
 */
void runInteractiveMode() {
    DataLoader loader;
    bool dataLoaded = false;
    int choice = -1;

    while (choice != 0) {
        cout << "\n=========================================================\n";
        cout << "      SISTEMA DE GESTÃO DE CONFERÊNCIAS\n";
        cout << "=========================================================\n";
        cout << "   [ DADOS E CONFIGURAÇÕES ]\n";
        cout << "1. Carregar Ficheiro de Dados (CSV)\n";
        cout << "2. Mostrar Lista de Submissões\n";
        cout << "3. Mostrar Lista de Revisores\n";
        cout << "4. Mostrar Parâmetros Atuais\n\n";
        cout << "   [ ALGORITMOS E OPERAÇÕES ]\n";
        cout << "5. Executar Atribuição de Revisões (Max-Flow)\n";
        cout << "6. Executar Análise de Risco (K-Failures)\n\n";
        cout << "0. Sair do Programa\n";
        cout << "=========================================================\n";
        cout << "Escolha uma opção: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada inválida. Por favor introduza um número válido.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                string filename;
                cout << "Nome do ficheiro (ex: dataset1.csv): ";
                cin >> filename;

                if (loader.loadData(filename)) {
                    cout << "Dados carregados com sucesso!\n";
                    cout << "Submissões lidas: " << loader.getSubmissions().size() << "\n";
                    cout << "Revisores lidos: " << loader.getReviewers().size() << "\n";
                    dataLoaded = true;
                } else {
                    cout << "Falha ao carregar ficheiro. Verifique o caminho e tente novamente.\n";
                }
                break;
            }
            case 2:
                if (!dataLoaded) {
                    cout << "Erro: Carregue os dados primeiro (Opção 1).\n";
                } else {
                    cout << "\n--- Submissões ---\n";
                    for (const auto& sub : loader.getSubmissions()) {
                        cout << "ID: " << sub.id << " | Título: " << sub.title << " | Domínio: " << sub.primaryDomain << "\n";
                    }
                }
                break;
            case 3:
                if (!dataLoaded) {
                    cout << "Erro: Carregue os dados primeiro (Opção 1).\n";
                } else {
                    cout << "\n--- Revisores ---\n";
                    for (const auto& rev : loader.getReviewers()) {
                        cout << "ID: " << rev.id << " | Nome: " << rev.name << " | Competência: " << rev.primaryExpertise << "\n";
                    }
                }
                break;
            case 4:
                if (!dataLoaded) {
                    cout << "Erro: Carregue os dados primeiro (Opção 1).\n";
                } else {
                    cout << "\n--- Parâmetros ---\n";
                    for (const auto& param : loader.getParameters()) {
                        cout << param.first << ": " << param.second << "\n";
                    }
                    cout << "\n--- Controlo ---\n";
                    for (const auto& line : loader.getControlData()) {
                        for (const auto& token : line) cout << token << ", ";
                        cout << "\n";
                    }
                }
                break;
            case 5:
                if (!dataLoaded) {
                    cout << "Erro: Carregue os dados primeiro (Opção 1).\n";
                } else {
                    executeReviewAssignment(loader);
                }
                break;
            case 6:
                if (!dataLoaded) {
                    cout << "Erro: Carregue os dados primeiro (Opção 1).\n";
                } else {
                    executeRiskAnalysis(loader);
                }
                break;
            case 0:
                cout << "A encerrar o programa...\n";
                break;
            default:
                cout << "Opção inválida. Por favor seleccione uma opção válida.\n";
                break;
        }
    }
}