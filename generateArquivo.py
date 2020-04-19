import os

lero_lero = '''Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard 
dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen 
book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially 
unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, 
and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.'''

"""
Gerar arquivos Dummy de diferentes tamanhos
"""
tam_10kb = 10240
tam_100kb = 102400
tam_1mb = 1048576
tam_100mb = 104857600
tam_1gb = 1073741824
tam_10gb = 10737418240


def gerando_arquivo(nm_arquivo, tam_arquivo, maior_arquivo=None):

    if maior_arquivo is not None:

        arquivo = open(maior_arquivo, 'r')
        conteudo = arquivo.readlines()

        # caso o tamanho não tenha chegado no tamanho o loop continua
        conteudo.append(lero_lero)
        arquivo = open(nm_arquivo, 'w')

        # escreve nova linha no arquivo
        arquivo.writelines(conteudo)
        arquivo.close()

    arquivo_size = os.path.getsize(nm_arquivo)

    while arquivo_size <= tam_arquivo:

        # pegando o que tem dentro do arquivo
        arquivo = open(nm_arquivo, 'r')
        conteudo = arquivo.readlines()

        # caso o tamanho não tenha chegado no tamanho o loop continua
        conteudo.append(lero_lero)
        arquivo = open(nm_arquivo, 'w')

        # escreve nova linha no arquivo
        arquivo.writelines(conteudo)
        arquivo.close()

        arquivo_size = os.path.getsize(nm_arquivo)


print("gerando arquivo 10kb")
gerando_arquivo('arquivo_10kb.txt', tam_10kb)

print("gerando arquivo 100kb")
gerando_arquivo('arquivo_100kb.txt', tam_100kb, 'arquivo_10kb.txt')

print("gerando arquivo 1mb")
gerando_arquivo('arquivo_1mb.txt', tam_1mb, 'arquivo_100kb.txt')

print("gerando arquivo 100mb")
gerando_arquivo('arquivo_100mb.txt', tam_100mb, 'arquivo_1mb.txt')

print("gerando arquivo 1gb")
gerando_arquivo('arquivo_1gb.txt', tam_1gb, 'arquivo_100mb.txt')

print("gerando arquivo 10gb")
gerando_arquivo('arquivo_10gb.txt', tam_10gb, 'arquivo_1gb.txt')