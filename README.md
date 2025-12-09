# Relatório do Projeto: Sistema de Gerenciamento Hospitalar

## Integrantes do Grupo

* **Cleyton Jose Rodrigues Macedo** - 16821725
* **Giovanni Torres Bullo** - 16869833
* **Guilherme Cavalcanti de Santana** - 15456556

---

## Justificativa das Estruturas de Dados Escolhidas

### Árvore AVL para o Registro de Pacientes

A escolha da Árvore AVL para o registro de pacientes é justificada pela necessidade de eficiência na busca, inserção e recuperação de dados, essencial para o sistema de um hospital onde tempo é crítico.

1.  **Complexidade O(log n) para busca:** A AVL garante que a altura da árvore seja sempre logarítmica em relação ao número de nós. A AVL garante a complexidade logarítmica pois seu balanceamento previne a degeneração da árvore.

2.  **Complexidade O(log n) para inserção:** Como a inserção de um novo nó na árvore envolve a busca (O(log n)), a inserção do nó (O(1)) e o processo de rebalanceamento (rotações, O(log n) no pior caso), a AVL tem custo total de O(log n), ou seja, é muito eficiente para inserções.

### Heap para a Fila de Prioridade

Devido a sua eficiência para lidar como valores de máximo e mínimo (como casos de emergência e casos não urgentes), a heap é excelente para ser utilizada como fila de prioridade.

1. **Acesso Imediato ao Mais Prioritário O(1):** Em uma emergência, o dado mais importante é quem é o próximo a ser atendido. A Heap mantém o elemento de maior prioridade sempre na raiz (índice 0 do vetor), permitindo acesso instantâneo.

2. **Eficiência na Inserção e Remoção O(log n):** Numa lista, ordenada ou não, as principais funções serão de tempo linear. A heap é uma estrutura que permite tanto inserção quanto busca e remoção em tempo O(log n), através das operações `fix_up` e `fix_down`.

3. **Estabilidade:** Pela forma como a função de comparação é construida, a heap garante que, se dois pacientes possuem o mesmo nível de prioridade, será atendido aquele que chegou primeiro.

## Estrutura Geral do Projeto

O desenvolvimento do projeto foi dividido em três etapas principais:

1.  **Criação de estruturas de dados genéricas:** Pilha, Árvore AVL (substituiu Lista) e Árvore Heap (substituiu Fila).
2.  **Criação de TADs específicos:** Tratamento e Paciente.
3.  **Desenvolvimento do sistema (`main.c`):** Integração dos TADs específicos com as estruturas genéricas.

> **Nota:** Como a terceira etapa foi realizada de forma independente e posterior às duas primeiras, algumas funções implementadas nas Estruturas de Dados (EDs) não foram utilizadas no sistema final.

### Dispatch Table

As estruturas de dados genéricas foram projetadas para armazenar ponteiros `void*` em vez de um tipo de dado pré-definido. Para que essas estruturas possam manipular os dados corretamente (ex: liberar memória, imprimir, salvar em disco), elas necessitam acessar funções específicas do tipo de dado armazenado.

Para solucionar isso, implementamos uma **Dispatch Table** definida em `tabela_funcoes.h`:

* A tabela armazena ponteiros para funções essenciais: `apagar`, `imprimir`, `comparar`, `salvar`, `carregar` e `copiar`.
* Os TADs específicos (`Paciente`, `Tratamento`) possuem uma instância estática dessa tabela preenchida com suas próprias funções e uma função "getter" para retornar o endereço dessa tabela.
* Ao criar uma ED genérica, o endereço da tabela correspondente é passado e armazenado na estrutura.

Isso permite que as EDs genéricas armazenem qualquer TAD específico (como Médico, Hospital, etc.), desde que as funções necessárias sejam fornecidas via Dispatch Table.

**Funções da Dispatch Table:**
* **Apagar e Imprimir:** Autoexplicativas.
* **Comparar:** Utilizada para buscas. Retorna `0` se dois itens forem considerados iguais (critério depende da implementação específica). A ED compara cada item armazenado com um item "chave" de busca.
* **Copiar:** Retorna uma cópia do item, útil para gerar listas de resultados de busca.
* **Salvar e Carregar:** Explicadas abaixo.

### Persistência de Dados (Salvar e Carregar)

Cada ED e TAD possui um par de funções `salvar` e `carregar` que trabalham em conjunto:

* **Salvar:** Recebe um ponteiro para o TAD/ED. Escreve um cabeçalho identificador, chama a função `salvar` dos itens armazenados (via Dispatch Table) recursivamente se necessário, e finaliza com um rodapé.
* **Carregar:** Lê o arquivo verificando o cabeçalho e rodapé para garantir a formatação correta. Reconstrói a estrutura na memória.

Essa abordagem encapsulada permite que a persistência de dados complexos (ex: uma lista de pacientes onde cada paciente tem uma pilha de tratamentos) seja feita de forma cooperativa entre os diferentes módulos.

### Gerenciamento de Memória

A política adotada define que as **EDs genéricas (Pilha, Fila, Lista) tornam-se proprietárias ("owners") dos itens inseridos**. Elas são responsáveis por chamar a função `apagar` de cada item (via Dispatch Table) quando a estrutura for destruída ou o item removido.

### Entrada de Dados (`entrada.h` / `entrada.c`)

O sistema utiliza um módulo dedicado para inputs do usuário. Ele define funções robustas para obter comandos, strings e números, implementando proteções contra *buffer overflow* e entradas inválidas, embora não garanta segurança total contra inputs maliciosos.

---

## Descrição de Cada TAD

* **PILHA:** É uma pilha encadeada de alocação dinâmica, que armazena ponteiros para `void` em nós contendo o item e um ponteiro para o anterior. Utiliza a Dispatch Table (`tabela_funcoes.h`) para manipular os dados genéricos. A função `pilha_salvar` utiliza recursão para percorrer até a base e gravar os dados da base para o topo, permitindo que a função `pilha_carregar` leia sequencialmente e reconstrua a pilha na ordem original.

* **HEAP:** Substituindo a Fila convencional, este TAD implementa uma Max-Heap sobre um vetor, garantindo acesso imediato ao elemento de maior prioridade. Cada nó armazena, além do item genérico, uma prioridade (`char`) e uma ordem de chegada (`int`), utilizada para desempatar elementos de mesma prioridade (critério `FIFO`). A função `heap_imprimir` realiza uma ordenação (`via qsort`) para exibir a lista completa por ordem de atendimento sem desfazer a estrutura da heap. A persistência salva os metadados de cada nó, permitindo restaurar o estado exato da fila, sem necessidade de utilizar a função de inserção durante o carregamento.

* **AVL:** Substituindo a Lista, é uma árvore binária de busca balanceada que armazena ponteiros para `void`. Cada nó mantém sua altura para calcular o fator de balanceamento e realizar rotações (esquerda/direita) automáticas na inserção e remoção. A função `avl_salvar` utiliza o caminho pré-ordem (raiz-esquerda-direita) e marca nós nulos com #. Isso permite que `avl_carregar` reconstrua a árvore com a exata topologia e balanceamento originais, sem necessidade de reprocessar as rotações durante o carregamento.

* **TRATAMENTO:** É um `struct` encapsulado que armazena uma string de até 100 caracteres representando um medicamento ou procedimento. A função `tratamento_criar` valida o tamanho da string para evitar buffer overflow. Possui uma função que retorna sua Dispatch Table específica, e sua função de comparação utiliza `strcmp` para verificar se dois tratamentos são idênticos.

* **PACIENTE:** É um struct composto que armazena um ID (inteiro), Nome (string) e uma `PILHA` interna para o histórico de tratamentos. Implementações específicas: `paciente_historico_cheio` limita o histórico a 10 itens e `paciente_comparar`´ retorna `0` se houver igualdade de ID ou de Nome, facilitando buscas flexíveis. As funções de persistência (`salvar`/`carregar`) gerenciam os dados do paciente e delegam a persistência do histórico para as funções do TAD Pilha.

---

## Sistema Propriamente Dito (`main.c`)

### Registro de Pacientes
O Registro é uma struct que gerencia:
1.  O próximo ID disponível (inteiro).
2.  Uma lista de pacientes.

* **Geração de IDs:** IDs são sequenciais e autoincrementais (iniciando em 1). Quando um paciente é inserido, o contador é incrementado. Isso evita colisões e tentativas manuais de encontrar IDs livres.
* **Óbito/Remoção:** Se um paciente falece e é removido, seu ID **não é reutilizado**. Isso previne inconsistências históricas e mistura de dados entre pacientes antigos e novos.
* **Persistência:** Possui funções próprias de salvar/carregar para armazenar tanto o contador de IDs quanto a lista completa (incluindo os históricos de tratamentos de cada paciente).

### Fila de Espera com Prioridade
* Implementada utilizando o TAD Heap.
* **Capacidade:** Máximo de 50 pacientes.
* **Regras:**
    * Se cheia, novos pacientes podem ser registrados no hospital, mas não podem entrar na fila de espera.
    * Não permite duplicidade (um paciente não pode estar duas vezes na fila).

### Fluxo de Execução
1.  **Inicialização:** O usuário informa os arquivos de registro e fila. O sistema verifica a existência e validade. Opção de iniciar do zero caso não existam.
2.  **Core Loop:** O usuário interage via menu de comandos numéricos.
    * `0`: Geralmente usado para cancelar operação ou Sair do sistema (no menu principal).
    * `1`: Confirmação em alguns contextos.
3.  **Saída:** Ao encerrar o sistema pelo menu principal, todos os dados (Registro e Fila) são salvos automaticamente nos arquivos correspondentes.
