#include "boolean.h"

void AndInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int source = getSourceValue(operand2, literal, modificator);
  int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin & source);
}

void OrInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int source = getSourceValue(operand2, literal, modificator);
  int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin | source);
}

void NotInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, !destin);
}

void XorInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int source = getSourceValue(operand2, literal, modificator);
  int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin ^ source);
}
