#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <emscripten/bind.h>

using namespace emscripten;

struct Card {
    std::string question;
    std::string answer;
};

class DiffEqReviewer {
private:
    std::vector<Card> currentDeck;
    size_t currentIndex;

public:
    DiffEqReviewer() : currentIndex(0) {
        srand(time(0));
    }

    void loadTerm(std::string term) {
        currentDeck.clear();
        currentIndex = 0;

        if (term == "prelim") {
            currentDeck.push_back({"What is a differential equation?", "An equation containing derivatives (or differentials) of one or more dependent variables"});
            currentDeck.push_back({"What is an ordinary differential equation (ODE)?", "A DE whose derivatives are with respect to a single independent variable"});
            currentDeck.push_back({"What is a partial differential equation (PDE)?", "A DE with partial derivatives with respect to two or more independent variables"});
            currentDeck.push_back({"What is the order of a differential equation?", "The order of the highest derivative in the equation"});
            currentDeck.push_back({"What is the degree of a differential equation?", "The power of the highest-order derivative"});
            currentDeck.push_back({"What is the form of a separable equation?", "dy/dx = f(x)g(y), or M(x)dx + N(y)dy = 0"});
            currentDeck.push_back({"Solve dy/dx = xy", "y = C e^(x^2/2)"});
            currentDeck.push_back({"What is the form of an exact equation?", "M(x,y)dx + N(x,y)dy = 0"});
            currentDeck.push_back({"What is the test for exactness?", "dM/dy = dN/dx (partial derivatives)"});
            currentDeck.push_back({"What is the standard form of a first-order linear equation?", "dy/dx + P(x)y = Q(x)"});
            currentDeck.push_back({"What is the integrating factor of a linear equation?", "mu = e^(int P(x) dx)"});
            currentDeck.push_back({"What is the form of Bernoulli's equation?", "dy/dx + P(x)y = Q(x)y^n"});
            currentDeck.push_back({"What substitution solves Bernoulli's equation?", "v = y^(1-n)"});
        } else if (term == "midterm") {
            currentDeck.push_back({"When is f(x,y) a homogeneous function of degree n?", "f(tx, ty) = t^n f(x,y)"});
            currentDeck.push_back({"What substitution solves a homogeneous DE?", "y = vx (so dy = v dx + x dv), or x = vy"});
            currentDeck.push_back({"What is the differential equation for growth and decay?", "dN/dt = kN"});
            currentDeck.push_back({"What is the solution of dN/dt = kN?", "N = N0 e^(kt)"});
            currentDeck.push_back({"State Newton's Law of Cooling as a DE", "dT/dt = k(T - Tm), where Tm is surrounding temperature"});
            currentDeck.push_back({"What is the basic balance equation for mixtures?", "dA/dt = rate in - rate out"});
            currentDeck.push_back({"State Newton's Second Law as a DE", "F = ma = m dv/dt"});
            currentDeck.push_back({"What is the terminal velocity?", "v = mg/k"});
            currentDeck.push_back({"What is the DE for an RL circuit?", "L di/dt + Ri = E(t)"});
            currentDeck.push_back({"What is the DE for an RC circuit?", "R dq/dt + q/C = E(t)"});
        } else if (term == "finals") {
            currentDeck.push_back({"What is the auxiliary equation of ay'' + by' + cy = 0?", "a m^2 + b m + c = 0"});
            currentDeck.push_back({"What is the solution for distinct real roots m1, m2?", "y = c1 e^(m1 x) + c2 e^(m2 x)"});
            currentDeck.push_back({"What is the solution for complex roots a +/- bi?", "y = e^(ax) (c1 cos bx + c2 sin bx)"});
            currentDeck.push_back({"Define the Laplace transform of f(t)", "L{f(t)} = int from 0 to infinity of e^(-st) f(t) dt"});
            currentDeck.push_back({"L{1} = ?", "1/s"});
            currentDeck.push_back({"L{t^n} = ?", "n! / s^(n+1)"});
            currentDeck.push_back({"L{e^(at)} = ?", "1 / (s - a)"});
            currentDeck.push_back({"L{sin kt} = ?", "k / (s^2 + k^2)"});
            currentDeck.push_back({"L{cos kt} = ?", "s / (s^2 + k^2)"});
            currentDeck.push_back({"L^-1{1/s} = ?", "1"});
            currentDeck.push_back({"L^-1{1/(s - a)} = ?", "e^(at)"});
            currentDeck.push_back({"What is Hooke's Law?", "F = kx"});
            currentDeck.push_back({"What is the DE for a spring-mass system?", "m x'' + c x' + k x = f(t)"});
        }

        for (int i = (int)currentDeck.size() - 1; i > 0; --i) {
            int j = rand() % (i + 1);
            std::swap(currentDeck[i], currentDeck[j]);
        }
    }

    std::string getQuestion() {
        if (currentDeck.empty()) return "Select a term to start.";
        return currentDeck[currentIndex].question;
    }

    std::string getAnswer() {
        if (currentDeck.empty()) return "";
        return currentDeck[currentIndex].answer;
    }

    int getCardIndex() {
        return (int)currentIndex + 1;
    }

    int getTotalCards() {
        return (int)currentDeck.size();
    }

    void nextCard() {
        if (!currentDeck.empty()) {
            currentIndex = (currentIndex + 1) % currentDeck.size();
        }
    }
};

EMSCRIPTEN_BINDINGS(diffeq_module) {
    class_<DiffEqReviewer>("DiffEqReviewer")
        .constructor<>()
        .function("loadTerm", &DiffEqReviewer::loadTerm)
        .function("getQuestion", &DiffEqReviewer::getQuestion)
        .function("getAnswer", &DiffEqReviewer::getAnswer)
        .function("getCardIndex", &DiffEqReviewer::getCardIndex)
        .function("getTotalCards", &DiffEqReviewer::getTotalCards)
        .function("nextCard", &DiffEqReviewer::nextCard);
}
