" required settings for Vundle
set  nocompatible 
filetype off

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'
Plugin 'Valloric/YouCompleteMe'
Plugin 'vim-scripts/Conque-GDB'
Plugin 'ctrlpvim/ctrlp.vim'
call vundle#end()
 
" Indentation
set tabstop=4
set shiftwidth=4
set softtabstop=4
set expandtab
set autoindent
set smartindent
set cindent

" Code readability
syntax on
set number
set colorcolumn=81

" read custom .vimrc from any folder
set exrc
" restrict certain commands
set secure

" exit insert mode with jj
inoremap jj <ESC>

" build tools
set makeprg='make debug'
nnoremap <F4> :make!<cr>
nnoremap <F5> :!pocket-home<cr>
nnoremap <f6> :ConqueGDB<cr>
