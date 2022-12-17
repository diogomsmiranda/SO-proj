# SO-proj

Functions tackled::

__state.c__

    inode_alloc() :: wr_lock para a inode table para alocar o inode

    inode_create() :: 
        wr_lock para a inode table no inicio;
        wr_lock para o inode a ser criado (para iniciar valores etc);
    
    data_block_alloc() :: wr lock para a data_blocks table

    data_block_get() :: rd lock para a data_blocks table

    data_block_free() :: wr_lock porque vamos dar free ao espaco na data_blocks_table

    inode_get() ::
        rd_lock para a inode table;

    clear_dir_entry() ::
        wr_lock porque estamos deletar aquela entrada;
        rd_block for the inode

    add_dir_entry() ::
        rd_lock para o inode em questao;

    find_in_dir() ::
        rd_lock para o node;
        rd_lock para a data_block table;

------------------

__operations.c__

    tfs_open() ::
        rd_lock para o node da root;
        wr_look para o node do file (porque existe a possibilidade de ser truncdo e termos de mudar o valor para 0);  

    tfs_symlink() ::
        rd_lock para o node da root;
        rd_lock para o node;
        wr_lock para a inode_table; 

    tfs_link() ::
        rd_lock para o node da root;
        wr_lock para o inode do file;

     
