#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

const int MAX_CONTAS = 100;
const int MAX_MOVIMENTACOES = 1000;

const string ARQUIVO_CONTAS = "contas.txt";
const string ARQUIVO_MOVIMENTACOES = "movimentacoes.txt";


int    numeroConta[MAX_CONTAS];
string nomeCliente[MAX_CONTAS];
string cpf[MAX_CONTAS];
int    tipoConta[MAX_CONTAS];      // 1 = Corrente, 2 = Poupança
double saldo[MAX_CONTAS];
int    contaAtiva[MAX_CONTAS];     // 1 = ativa, 0 = inativa
int    quantidadeContas = 0;


int    contaMovimentacao[MAX_MOVIMENTACOES];
char   tipoMovimentacao[MAX_MOVIMENTACOES];   // 'D' ou 'S'
double valorMovimentacao[MAX_MOVIMENTACOES];
int    quantidadeMovimentacoes = 0;



int buscarConta(int numero) {
    for (int i = 0; i < quantidadeContas; i++) {
        if (numeroConta[i] == numero) return i;
    }
    return -1;
}

void limparEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int lerInteiro(const string& mensagem) {
    int valor;
    cout << mensagem;
    while (!(cin >> valor)) {
        cout << "Entrada invalida. Tente novamente: ";
        limparEntrada();
    }
    limparEntrada();
    return valor;
}

double lerValor(const string& mensagem) {
    double valor;
    cout << mensagem;
    while (!(cin >> valor)) {
        cout << "Entrada invalida. Tente novamente: ";
        limparEntrada();
    }
    limparEntrada();
    return valor;
}

string nomeTipo(int tipo) {
    return (tipo == 1) ? "Corrente" : "Poupanca";
}



void salvarContas() {
    ofstream arquivo(ARQUIVO_CONTAS);   // sobrescreve com o estado atual dos arrays
    if (!arquivo.is_open()) {
        cout << "ERRO: nao foi possivel abrir " << ARQUIVO_CONTAS << " para gravacao.\n";
        return;
    }
    arquivo << fixed << setprecision(2);
    for (int i = 0; i < quantidadeContas; i++) {
        arquivo << numeroConta[i] << ";"
                << nomeCliente[i] << ";"
                << cpf[i] << ";"
                << tipoConta[i] << ";"
                << saldo[i] << ";"
                << contaAtiva[i] << endl;
    }
    arquivo.close();
}

void salvarMovimentacoes() {
    ofstream arquivo(ARQUIVO_MOVIMENTACOES);
    if (!arquivo.is_open()) {
        cout << "ERRO: nao foi possivel abrir " << ARQUIVO_MOVIMENTACOES << " para gravacao.\n";
        return;
    }
    arquivo << fixed << setprecision(2);
    for (int i = 0; i < quantidadeMovimentacoes; i++) {
        arquivo << contaMovimentacao[i] << ";"
                << tipoMovimentacao[i] << ";"
                << valorMovimentacao[i] << endl;
    }
    arquivo.close();
}



void carregarContas() {
    quantidadeContas = 0;
    ifstream arquivo(ARQUIVO_CONTAS);
    if (!arquivo.is_open()) {
        return;   // primeira execução: arquivo ainda não existe
    }

    string linha;
    while (getline(arquivo, linha) && quantidadeContas < MAX_CONTAS) {
        if (linha.empty()) continue;

        stringstream ss(linha);
        string campo[6];
        int n = 0;
        while (n < 6 && getline(ss, campo[n], ';')) n++;
        if (n < 6) continue;   // linha incompleta: ignora

        try {
            int i = quantidadeContas;
            numeroConta[i] = stoi(campo[0]);
            nomeCliente[i] = campo[1];
            cpf[i]         = campo[2];
            tipoConta[i]   = stoi(campo[3]);
            saldo[i]       = stod(campo[4]);
            contaAtiva[i]  = stoi(campo[5]);
            quantidadeContas++;
        } catch (...) {
            
        }
    }
    arquivo.close();
}

void carregarMovimentacoes() {
    quantidadeMovimentacoes = 0;
    ifstream arquivo(ARQUIVO_MOVIMENTACOES);
    if (!arquivo.is_open()) {
        return;
    }

    string linha;
    while (getline(arquivo, linha) && quantidadeMovimentacoes < MAX_MOVIMENTACOES) {
        if (linha.empty()) continue;

        stringstream ss(linha);
        string campo[3];
        int n = 0;
        while (n < 3 && getline(ss, campo[n], ';')) n++;
        if (n < 3 || campo[1].empty()) continue;

        try {
            int i = quantidadeMovimentacoes;
            contaMovimentacao[i] = stoi(campo[0]);
            tipoMovimentacao[i]  = campo[1][0];
            valorMovimentacao[i] = stod(campo[2]);
            quantidadeMovimentacoes++;
        } catch (...) {
        }
    }
    arquivo.close();
}


void registrarMovimentacao(int numero, char tipo, double valor) {
    if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
        cout << "Limite de movimentacoes atingido!\n";
        return;
    }
    contaMovimentacao[quantidadeMovimentacoes] = numero;
    tipoMovimentacao[quantidadeMovimentacoes]  = tipo;
    valorMovimentacao[quantidadeMovimentacoes] = valor;
    quantidadeMovimentacoes++;
}



void cadastrarConta() {
    if (quantidadeContas >= MAX_CONTAS) {
        cout << "Limite de contas atingido!\n";
        return;
    }

    int numero = lerInteiro("Numero da conta: ");
    if (numero <= 0) {
        cout << "Numero de conta invalido!\n";
        return;
    }
    if (buscarConta(numero) != -1) {
        cout << "Ja existe uma conta com esse numero!\n";
        return;
    }

    string nome, documento;
    cout << "Nome do cliente: ";
    getline(cin, nome);
    if (nome.empty()) {
        cout << "Nome nao pode ser vazio!\n";
        return;
    }
    
    for (char& c : nome) if (c == ';') c = ' ';

    cout << "CPF (somente numeros): ";
    getline(cin, documento);
    for (char& c : documento) if (c == ';') c = ' ';

    int tipo;
    do {
        tipo = lerInteiro("Tipo da conta (1 - Corrente, 2 - Poupanca): ");
    } while (tipo != 1 && tipo != 2);

    double saldoInicial = lerValor("Saldo inicial: R$ ");
    if (saldoInicial < 0) {
        cout << "Saldo inicial nao pode ser negativo!\n";
        return;
    }

    int i = quantidadeContas;
    numeroConta[i] = numero;
    nomeCliente[i] = nome;
    cpf[i]         = documento;
    tipoConta[i]   = tipo;
    saldo[i]       = saldoInicial;
    contaAtiva[i]  = 1;
    quantidadeContas++;

    if (saldoInicial > 0) {
        registrarMovimentacao(numero, 'D', saldoInicial);
        salvarMovimentacoes();
    }
    salvarContas();

    cout << "Conta cadastrada com sucesso!\n";
}

void imprimirConta(int i) {
    cout << "Conta: " << numeroConta[i]
         << " | Cliente: " << nomeCliente[i]
         << " | CPF: " << cpf[i]
         << " | Tipo: " << nomeTipo(tipoConta[i])
         << " | Saldo: R$ " << fixed << setprecision(2) << saldo[i]
         << " | Situacao: " << (contaAtiva[i] == 1 ? "Ativa" : "Inativa") << "\n";
}

void listarContas() {
    if (quantidadeContas == 0) {
        cout << "Nenhuma conta cadastrada.\n";
        return;
    }
    cout << "\n--- CONTAS CADASTRADAS ---\n";
    for (int i = 0; i < quantidadeContas; i++) {
        imprimirConta(i);
    }
}

void consultarConta() {
    int numero = lerInteiro("Numero da conta: ");
    int i = buscarConta(numero);
    if (i == -1) {
        cout << "Conta nao encontrada!\n";
        return;
    }
    imprimirConta(i);
}

void depositar() {
    int numero = lerInteiro("Numero da conta: ");
    int i = buscarConta(numero);
    if (i == -1) {
        cout << "Conta nao encontrada!\n";
        return;
    }
    if (contaAtiva[i] != 1) {
        cout << "Conta inativa!\n";
        return;
    }

    double valor = lerValor("Valor do deposito: R$ ");
    if (valor <= 0) {
        cout << "Valor invalido!\n";
        return;
    }

    saldo[i] += valor;
    registrarMovimentacao(numero, 'D', valor);
    salvarContas();
    salvarMovimentacoes();

    cout << "Deposito realizado! Novo saldo: R$ " << fixed << setprecision(2) << saldo[i] << "\n";
}

void sacar() {
    int numero = lerInteiro("Numero da conta: ");
    int i = buscarConta(numero);
    if (i == -1) {
        cout << "Conta nao encontrada!\n";
        return;
    }
    if (contaAtiva[i] != 1) {
        cout << "Conta inativa!\n";
        return;
    }

    double valor = lerValor("Valor do saque: R$ ");
    if (valor <= 0) {
        cout << "Valor invalido!\n";
        return;
    }
    if (valor > saldo[i]) {
        cout << "Saldo insuficiente!\n";
        return;
    }

    saldo[i] -= valor;
    registrarMovimentacao(numero, 'S', valor);
    salvarContas();
    salvarMovimentacoes();

    cout << "Saque realizado! Novo saldo: R$ " << fixed << setprecision(2) << saldo[i] << "\n";
}

void transferir() {
    int origem = lerInteiro("Conta de origem: ");
    int destino = lerInteiro("Conta de destino: ");

    if (origem == destino) {
        cout << "As contas de origem e destino devem ser diferentes!\n";
        return;
    }

    int io = buscarConta(origem);
    int id = buscarConta(destino);
    if (io == -1 || id == -1) {
        cout << "Conta de origem ou destino nao encontrada!\n";
        return;
    }
    if (contaAtiva[io] != 1 || contaAtiva[id] != 1) {
        cout << "Uma das contas esta inativa!\n";
        return;
    }

    double valor = lerValor("Valor da transferencia: R$ ");
    if (valor <= 0) {
        cout << "Valor invalido!\n";
        return;
    }
    if (valor > saldo[io]) {
        cout << "Saldo insuficiente na conta de origem!\n";
        return;
    }
    if (quantidadeMovimentacoes + 2 > MAX_MOVIMENTACOES) {
        cout << "Limite de movimentacoes atingido!\n";
        return;
    }

    saldo[io] -= valor;
    saldo[id] += valor;
    registrarMovimentacao(origem, 'S', valor);    // saída na origem
    registrarMovimentacao(destino, 'D', valor);   // entrada no destino
    salvarContas();
    salvarMovimentacoes();

    cout << "Transferencia realizada com sucesso!\n";
}

void exibirExtrato() {
    int numero = lerInteiro("Numero da conta: ");
    int i = buscarConta(numero);
    if (i == -1) {
        cout << "Conta nao encontrada!\n";
        return;
    }

    cout << "\n--- EXTRATO DA CONTA " << numero << " (" << nomeCliente[i] << ") ---\n";
    bool encontrou = false;
    for (int k = 0; k < quantidadeMovimentacoes; k++) {
        if (contaMovimentacao[k] == numero) {
            cout << (tipoMovimentacao[k] == 'D' ? "Deposito/Entrada: " : "Saque/Saida:      ")
                 << "R$ " << fixed << setprecision(2) << valorMovimentacao[k] << "\n";
            encontrou = true;
        }
    }
    if (!encontrou) cout << "Nenhuma movimentacao registrada.\n";
    cout << "Saldo atual: R$ " << fixed << setprecision(2) << saldo[i] << "\n";
}



int main() {
    carregarContas();
    carregarMovimentacoes();

    int opcao;
    do {
        cout << "\n===== SISTEMA BANCARIO =====\n"
             << "1 - Cadastrar conta\n"
             << "2 - Listar contas\n"
             << "3 - Consultar conta\n"
             << "4 - Depositar\n"
             << "5 - Sacar\n"
             << "6 - Transferir\n"
             << "7 - Extrato\n"
             << "0 - Sair\n";
        opcao = lerInteiro("Opcao: ");

        switch (opcao) {
            case 1: cadastrarConta(); break;
            case 2: listarContas();   break;
            case 3: consultarConta(); break;
            case 4: depositar();      break;
            case 5: sacar();          break;
            case 6: transferir();     break;
            case 7: exibirExtrato();  break;
            case 0:
                salvarContas();
                salvarMovimentacoes();
                cout << "Dados salvos. Encerrando...\n";
                break;
            default: cout << "Opcao invalida!\n";
        }
    } while (opcao != 0);

    return 0;
}
