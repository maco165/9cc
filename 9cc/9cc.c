#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// �g�[�N���̎��
typedef enum {
  TK_RESERVED, // �L��
  TK_NUM,      // �����g�[�N��
  TK_EOF,      // ���͂̏I����\���g�[�N��
} TokenKind;

typedef struct Token Token;

// �g�[�N���^
struct Token {
  TokenKind kind; // �g�[�N���̌^
  Token *next;    // ���̓��̓g�[�N��
  int val;        // kind��TK_NUM�̏ꍇ�A���̐��l
  char *str;      // �g�[�N��������
};

// ���ݒ��ڂ��Ă���g�[�N��
Token *token;

// �G���[��񍐂��邽�߂̊֐�
// printf�Ɠ������������
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// ���̃g�[�N�������҂��Ă���L���̂Ƃ��ɂ́A�g�[�N����1�ǂݐi�߂�
// �^��Ԃ��B����ȊO�̏ꍇ�ɂ͋U��Ԃ��B
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    return false;
  token = token->next;
  return true;
}

// ���̃g�[�N�������҂��Ă���L���̂Ƃ��ɂ́A�g�[�N����1�ǂݐi�߂�B
// ����ȊO�̏ꍇ�ɂ̓G���[��񍐂���B
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error("'%c'�ł͂���܂���", op);
  token = token->next;
}

// ���̃g�[�N�������l�̏ꍇ�A�g�[�N����1�ǂݐi�߂Ă��̐��l��Ԃ��B
// ����ȊO�̏ꍇ�ɂ̓G���[��񍐂���B
int expect_number() {
  if (token->kind != TK_NUM)
    error("���ł͂���܂���");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

// �V�����g�[�N�����쐬����cur�Ɍq����
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// ���͕�����p���g�[�N�i�C�Y���Ă����Ԃ�
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // �󔒕������X�L�b�v
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error("�g�[�N�i�C�Y�ł��܂���");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    error("�����̌�������������܂���");
    return 1;
  }

  // �g�[�N�i�C�Y����
  token = tokenize(argv[1]);

  // �A�Z���u���̑O���������o��
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // ���̍ŏ��͐��łȂ���΂Ȃ�Ȃ��̂ŁA������`�F�b�N����
  // �ŏ���mov���߂��o��
  printf("  mov rax, %d\n", expect_number());

  // `+ <��>`���邢��`- <��>`�Ƃ����g�[�N���̕��т������
  // �A�Z���u�����o��
  while (!at_eof()) {
    if (consume('+')) {
      printf("  add rax, %d\n", expect_number());
      continue;
    }

    expect('-');
    printf("  sub rax, %d\n", expect_number());
  }

  printf("  ret\n");
  return 0;
}