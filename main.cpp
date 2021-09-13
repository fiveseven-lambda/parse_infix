#include <iostream>
#include <memory>
#include <optional>

enum class Op {
	Mul, Div,
	Add, Sub,
	Less, Greater,
	And,
	Or,
};

class Expr {
public:
	virtual void print(int = 0) = 0;
	virtual ~Expr() = default;
};

class Id: public Expr {
	char name;
public:
	Id(char name): name(name) {}
	virtual void print(int) override;
	~Id() = default;
};

class Binary : public Expr {
	std::unique_ptr<Expr> left, right;
	Op op;
public:
	Binary(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, Op op):
		left(std::move(left)),
		right(std::move(right)),
		op(op) {}
	virtual void print(int) override;
	~Binary() = default;
};

std::unique_ptr<Expr> parse(int precedence = 0) {
	if(precedence == 5) return std::make_unique<Id>(std::cin.get());
	std::unique_ptr<Expr> ret = parse(precedence + 1);
	for(;;){
		std::optional<Op> op;
		switch(std::cin.peek()){
			case '|':
				if(precedence == 0) op = Op::Or;
				break;
			case '&':
				if(precedence == 1) op = Op::And;
				break;
			case '<':
				if(precedence == 2) op = Op::Less;
				break;
			case '>':
				if(precedence == 2) op = Op::Greater;
				break;
			case '+':
				if(precedence == 3) op = Op::Add;
				break;
			case '-':
				if(precedence == 3) op = Op::Sub;
				break;
			case '*':
				if(precedence == 4) op = Op::Mul;
				break;
			case '/':
				if(precedence == 4) op = Op::Div;
				break;
		}
		if(!op) return ret;
		std::cin.get();
		ret = std::make_unique<Binary>(
			std::move(ret),
			parse(precedence + 1),
			op.value()
		);
	}
}

int main(){
	parse()->print();
}

constexpr int TAB = 4;

void Id::print(int indent) {
	for(int i = 0; i < indent; ++i) std::cout << ' ';
	std::cout << name << std::endl;
}

void Binary::print(int indent) {
	for(int i = 0; i < indent; ++i) std::cout << ' ';
	std::string name;
	switch(op){
		case Op::Mul:
			name = "mul";
			break;
		case Op::Div:
			name = "div";
			break;
		case Op::Add:
			name = "add";
			break;
		case Op::Sub:
			name = "sub";
			break;
		case Op::Less:
			name = "less";
			break;
		case Op::Greater:
			name = "greater";
			break;
		case Op::And:
			name = "and";
			break;
		case Op::Or:
			name = "or";
	}
	std::cout << name << std::endl;
	left->print(indent + TAB);
	right->print(indent + TAB);
}
