#pragma once
#include <string>
#include <exception>
#include "Structures.h"

using namespace std;

class Integer : public List<uint8_t> {
private:
	bool isNegative;

protected:
	void appendDigit(uint8_t newDigit) {
		DLLNode<uint8_t>* newNode = new DLLNode<uint8_t>(newDigit);
		if (this->length() < this->getMaxLen()) {
			if (this->getStart() == nullptr) {
				this->setStart(newNode);
				this->setEnd(this->getStart());
			}
			else {
				this->getStart()->setPrevious(newNode);
				newNode->setNext(this->getStart());
				this->setStart(newNode);
			}
		}
	}

public:
	// ========== constructors ==========
	Integer() {
		this->setStart(nullptr);
		this->setEnd(nullptr);
		this->isNegative = false;
		this->setLength(0);
		return;
	}

	Integer(string number) {
		if (number.length() > 0) {
			// first element needs to be added to the list
			// it will be both the start and the end
			if (number[0] >= '0' && number[0] <= '9') {
				this->setStart(new DLLNode<uint8_t>(number[0] - 48));
				this->setEnd(this->getStart());
				this->setLength(1);
			}
			else {
				throw invalid_argument("Argument provided contains non-numeric characters.");
			}

			// each subsequent digit should be added to the end
			// so the integer reads from left to right
			for (int i = 1; i < number.length(); i++) {
				if (number[i] >= '0' && number[i] <= '9') {
					this->setEnd(new DLLNode<uint8_t>(this->getEnd(), number[i] - 48));
					this->getEnd()->getPrevious()->setNext(this->getEnd());
				}
				else {
					throw invalid_argument("Arugment provided contains non-numeric characters");
				}
			}
		}
		this->isNegative = false;
		return;
	}

	// ========== utility ==========
	void print() {
		DLLNode<uint8_t>* element = this->getStart();
		while (element != nullptr) {
			cout << (int)element->getPayload();
			element = element->getNext();
		}
	}

	Integer operator+(Integer& i) {
		DLLNode<uint8_t>* thisPtr = getEnd();
		DLLNode<uint8_t>* iPtr = i.getEnd();
		Integer newNumber;
		uint8_t digit = 0; 
		bool carry = 0; 

		while (thisPtr != nullptr && iPtr != nullptr) {
			digit = thisPtr->getPayload() + iPtr->getPayload() + carry;
			carry = digit >= 10;
			if (carry) {
				digit -= 10;
			}
			newNumber.appendDigit(digit);
			newNumber.setLength(newNumber.length() + 1);
			thisPtr = thisPtr->getPrevious();
			iPtr = iPtr->getPrevious();
		}

		while (thisPtr != nullptr) {
			newNumber.appendDigit(thisPtr->getPayload() + carry);
			carry = 0;
			newNumber.setLength(newNumber.length() + 1);
			thisPtr = thisPtr->getPrevious();
		}

		while (iPtr != nullptr) {
			newNumber.appendDigit(thisPtr->getPayload() + carry);
			carry = 0;
			newNumber.setLength(newNumber.length() + 1);
			iPtr = iPtr->getPrevious();
		}
		
		if (carry) {
			newNumber.appendDigit(1);
		}

		return newNumber;
	}
};
