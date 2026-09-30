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

struct Topic {
    std::string name;
    std::vector<Card> cards;
};

class DiffEqReviewer {
private:
    std::vector<Topic> prelimTopics;
    std::vector<Topic> midtermTopics;
    std::vector<Topic> finalsTopics;

    std::vector<Card> currentDeck;
    size_t currentIndex;

    void initData() {
        // PRELIM TOPICS
        prelimTopics = {
            {"Differential Equations and Its Classifications", {
                {"What is a differential equation?", "An equation containing derivatives (or differentials) of one or more dependent variables"},
                {"What is an ordinary differential equation (ODE)?", "A DE whose derivatives are with respect to a single independent variable"},
                {"What is a partial differential equation (PDE)?", "A DE with partial derivatives with respect to two or more independent variables"},
                {"What is the order of a differential equation?", "The order of the highest derivative in the equation"},
                {"What is the degree of a differential equation?", "The power of the highest-order derivative (after clearing radicals and fractions of derivatives)"},
                {"Give the order and degree of (y'')^2 + (y')^3 = x", "Order 2, degree 2"},
                {"When is a DE linear?", "The dependent variable and its derivatives appear only to the first power, are not multiplied together, and coefficients depend only on the independent variable"},
                {"Is y' + y^2 = x linear or nonlinear?", "Nonlinear, because of the y^2 term"}
            }},
            {"Verifying Solutions of Differential Equations", {
                {"How do you verify that a function is a solution of a DE?", "Differentiate it as many times as the order, substitute into the DE, and check that both sides are equal"},
                {"What is a general solution?", "A solution containing n arbitrary constants for an nth-order DE"},
                {"What is a particular solution?", "A solution obtained by assigning values to the arbitrary constants (e.g. using initial conditions)"},
                {"What is a singular solution?", "A solution that cannot be obtained from the general solution by any choice of constants"},
                {"How many arbitrary constants are in the general solution of a 3rd-order DE?", "3"},
                {"Is y = e^(2x) a solution of y' - 2y = 0?", "Yes. y' = 2e^(2x), so 2e^(2x) - 2e^(2x) = 0"},
                {"Is y = x^2 a solution of y' = 2y/x?", "Yes. y' = 2x and 2y/x = 2x"},
                {"What is an initial value problem (IVP)?", "A DE together with conditions on y and its derivatives given at a single point"}
            }},
            {"Separable Equations", {
                {"What is the form of a separable equation?", "dy/dx = f(x)g(y), or M(x)dx + N(y)dy = 0"},
                {"What is the method for solving a separable equation?", "Get all y-terms with dy and all x-terms with dx, then integrate both sides"},
                {"Solve dy/dx = xy", "y = C e^(x^2/2)"},
                {"Solve dy/dx = y/x", "y = Cx"},
                {"Solve dy/dx = -x/y", "x^2 + y^2 = C"},
                {"What is the integral of dy/y?", "ln|y| + C"},
                {"What can be lost when you divide by g(y) while separating variables?", "Constant solutions where g(y) = 0"}
            }},
            {"Exact Equations", {
                {"What is the form of an exact equation?", "M(x,y)dx + N(x,y)dy = 0"},
                {"What is the test for exactness?", "dM/dy = dN/dx (partial derivatives)"},
                {"What is the solution of an exact equation?", "F(x,y) = C, where dF/dx = M and dF/dy = N"},
                {"What are the steps to solve an exact equation?", "Integrate M with respect to x and add g(y); differentiate with respect to y and set equal to N; solve for g(y)"},
                {"Is (2xy)dx + (x^2)dy = 0 exact? Solve it.", "Yes, dM/dy = 2x = dN/dx. Solution: x^2 y = C"},
                {"Is y dx + 3x dy = 0 exact?", "No. dM/dy = 1 but dN/dx = 3"}
            }},
            {"Non-Exact Equations", {
                {"What is an integrating factor?", "A function that, when multiplied to a non-exact DE, makes it exact"},
                {"When is the integrating factor a function of x only?", "When (dM/dy - dN/dx)/N depends on x only"},
                {"What is the integrating factor when (dM/dy - dN/dx)/N depends on x only?", "mu(x) = e^(int [(dM/dy - dN/dx)/N] dx)"},
                {"When is the integrating factor a function of y only?", "When (dN/dx - dM/dy)/M depends on y only"},
                {"What is the integrating factor when (dN/dx - dM/dy)/M depends on y only?", "mu(y) = e^(int [(dN/dx - dM/dy)/M] dy)"},
                {"Give common integrating factors for y dx - x dy = 0", "1/x^2, 1/y^2, or 1/(xy); the solution is y/x = C"},
                {"What should you do after multiplying by the integrating factor?", "Re-check that dM/dy = dN/dx, then solve as an exact equation"}
            }},
            {"Linear Equations", {
                {"What is the standard form of a first-order linear equation?", "dy/dx + P(x)y = Q(x)"},
                {"What is the integrating factor of a linear equation?", "mu = e^(int P(x) dx)"},
                {"What is the general solution of a linear equation?", "y * mu = int (mu * Q dx) + C"},
                {"What is the first step in solving a linear equation?", "Divide by the coefficient of y' to reach standard form"},
                {"Solve y' + 2y = 0", "y = C e^(-2x)"},
                {"Find the integrating factor of y' + (1/x)y = x, then solve", "mu = x; solution: xy = x^3/3 + C"},
                {"What if the equation is linear in x instead of y?", "Treat x as dependent: dx/dy + P(y)x = Q(y)"}
            }},
            {"Bernoulli's Equations", {
                {"What is the form of Bernoulli's equation?", "dy/dx + P(x)y = Q(x)y^n"},
                {"What substitution solves Bernoulli's equation?", "v = y^(1-n)"},
                {"What linear equation results from v = y^(1-n)?", "dv/dx + (1-n)P(x)v = (1-n)Q(x)"},
                {"What does Bernoulli's equation reduce to when n = 0 or n = 1?", "n = 0: linear equation; n = 1: separable equation"},
                {"What substitution solves y' + y = xy^2?", "v = y^(-1), giving dv/dx - v = -x"},
                {"What is the final step after solving for v?", "Substitute back v = y^(1-n) to get y"}
            }}
        };

        // MIDTERM TOPICS
        midtermTopics = {
            {"Homogeneous Equations", {
                {"When is f(x,y) a homogeneous function of degree n?", "f(tx, ty) = t^n f(x,y)"},
                {"When is M dx + N dy = 0 a homogeneous DE?", "When M and N are homogeneous functions of the same degree"},
                {"What substitution solves a homogeneous DE?", "y = vx (so dy = v dx + x dv), or x = vy"},
                {"What type of equation results after substituting y = vx?", "A separable equation in v and x"},
                {"Is (x^2 + y^2)dx + xy dy = 0 homogeneous?", "Yes, both M and N have degree 2"},
                {"What is the last step after integrating?", "Replace v with y/x (or x/y)"}
            }},
            {"Linear in Two Variables", {
                {"What is the form of a DE with linear coefficients?", "(a1 x + b1 y + c1)dx + (a2 x + b2 y + c2)dy = 0"},
                {"What do you do if a1/a2 is not equal to b1/b2 (lines intersect)?", "Find the intersection (h,k), substitute x = u + h, y = w + k to get a homogeneous equation"},
                {"How do you find (h,k)?", "Solve a1 x + b1 y + c1 = 0 and a2 x + b2 y + c2 = 0 simultaneously"},
                {"What do you do if a1/a2 = b1/b2 (parallel lines)?", "Let z = a1 x + b1 y, which gives a separable equation"}
            }},
            {"Growth and Decay", {
                {"What is the differential equation for growth and decay?", "dN/dt = kN"},
                {"What is the solution of dN/dt = kN?", "N = N0 e^(kt)"},
                {"What is the sign of k for growth? For decay?", "Growth: k > 0. Decay: k < 0"},
                {"What is the doubling time?", "t = ln 2 / k"},
                {"What is the half-life?", "t = ln 2 / |k|"},
                {"How do you find k?", "Use N0 and one other known value of N at a known time, then solve for k"},
                {"A population of 1000 doubles in 3 hours. What is it after 6 hours?", "4000"}
            }},
            {"Newton's Law of Cooling", {
                {"State Newton's Law of Cooling as a DE", "dT/dt = k(T - Tm), where Tm is the surrounding temperature"},
                {"What is the solution of Newton's Law of Cooling?", "T = Tm + (T0 - Tm) e^(kt)"},
                {"What is the sign of k in cooling problems?", "Negative (k < 0)"},
                {"What does T approach as t goes to infinity?", "The surrounding temperature Tm"},
                {"What are the usual steps in solving cooling problems?", "Find k from a known second temperature reading, then use T(t) to find the required time or temperature"}
            }},
            {"Mixture Problems", {
                {"What is the basic balance equation for mixtures?", "dA/dt = rate in - rate out"},
                {"How do you compute the rate in?", "(concentration in) x (flow rate in)"},
                {"How do you compute the rate out?", "(A / V(t)) x (flow rate out)"},
                {"What is V(t) if flow in is r_in and flow out is r_out?", "V(t) = V0 + (r_in - r_out)t"},
                {"What is the linear form of a mixture problem?", "dA/dt + (r_out / V) A = c_in r_in"},
                {"A 100 L tank of pure water receives 0.5 kg/L brine at 2 L/min and drains at 2 L/min. Find A(t).", "A(t) = 50(1 - e^(-0.02t)) kg"},
                {"What is the limiting amount of salt in that tank?", "50 kg"}
            }},
            {"Mechanics Problems", {
                {"State Newton's Second Law as a DE", "F = ma = m dv/dt"},
                {"How are velocity and acceleration related to position s?", "v = ds/dt and a = dv/dt = v dv/ds"},
                {"Write the DE for a falling body with air resistance proportional to velocity", "m dv/dt = mg - kv"},
                {"What is the terminal velocity?", "v = mg/k"},
                {"Solve m dv/dt = mg - kv with v(0) = 0", "v(t) = (mg/k)(1 - e^(-kt/m))"},
                {"What is the relationship between weight and mass?", "W = mg"}
            }},
            {"Electrical Circuits", {
                {"State Kirchhoff's Voltage Law", "The sum of voltage drops around a closed loop equals the applied voltage"},
                {"What is the voltage drop across a resistor? Inductor? Capacitor?", "Resistor: Ri. Inductor: L di/dt. Capacitor: q/C"},
                {"What is the DE for an RL circuit?", "L di/dt + Ri = E(t)"},
                {"What is the DE for an RC circuit?", "R dq/dt + q/C = E(t)"},
                {"How are current and charge related?", "i = dq/dt"},
                {"Solve the RL circuit with constant E and i(0) = 0", "i = (E/R)(1 - e^(-Rt/L))"},
                {"Solve the RC circuit with constant E and q(0) = 0", "q = EC(1 - e^(-t/RC))"},
                {"What is the time constant of an RL circuit? RC circuit?", "RL: L/R. RC: RC"}
            }}
        };

        // FINALS TOPICS
        finalsTopics = {
            {"Introduction to Higher-Order Differential Equations", {
                {"What is the general form of an nth-order linear DE?", "a_n(x) y^(n) + ... + a_1(x) y' + a_0(x) y = g(x)"},
                {"How many initial conditions does an nth-order IVP need?", "n"},
                {"What is the general solution of a homogeneous linear DE?", "y = c1 y1 + c2 y2 + ... + cn yn (linear combination of n independent solutions)"},
                {"What is the Wronskian of two functions y1 and y2?", "W = y1 y2' - y2 y1'"},
                {"What does W not equal to 0 tell you?", "The solutions are linearly independent"},
                {"State the superposition principle", "Any linear combination of solutions of a homogeneous linear DE is also a solution"},
                {"What is the general solution of a non-homogeneous DE?", "y = yc + yp (complementary solution plus particular solution)"},
                {"What does the operator D stand for?", "D = d/dx"}
            }},
            {"Homogeneous with Constant Coefficient", {
                {"What is the auxiliary (characteristic) equation of ay'' + by' + cy = 0?", "a m^2 + b m + c = 0"},
                {"What is the solution for distinct real roots m1, m2?", "y = c1 e^(m1 x) + c2 e^(m2 x)"},
                {"What is the solution for a repeated root m?", "y = (c1 + c2 x) e^(mx)"},
                {"What is the solution for complex roots a +/- bi?", "y = e^(ax) (c1 cos bx + c2 sin bx)"},
                {"Solve y'' - 5y' + 6y = 0", "y = c1 e^(2x) + c2 e^(3x)"},
                {"Solve y'' + 4y = 0", "y = c1 cos 2x + c2 sin 2x"},
                {"Solve y'' - 2y' + y = 0", "y = (c1 + c2 x) e^x"},
                {"What is the solution for a root m repeated k times?", "y = (c1 + c2 x + ... + ck x^(k-1)) e^(mx)"}
            }},
            {"Non-Homogeneous with Arbitrary Coefficient (Undetermined Coefficients)", {
                {"What is the form of the general solution of a non-homogeneous DE?", "y = yc + yp"},
                {"What is the trial yp if g(x) is a polynomial of degree n?", "A_n x^n + ... + A_1 x + A_0"},
                {"What is the trial yp if g(x) = e^(ax)?", "A e^(ax)"},
                {"What is the trial yp if g(x) = sin bx or cos bx?", "A cos bx + B sin bx"},
                {"What is the modification rule?", "If the trial yp duplicates a term of yc, multiply it by x^s (smallest s that removes the duplication)"},
                {"Find yp for y'' - 3y' + 2y = e^(3x)", "yp = (1/2) e^(3x)"},
                {"What is the trial yp for y'' - y = e^x?", "yp = A x e^x (e^x already appears in yc)"}
            }},
            {"Variation of Parameters", {
                {"What is the form of yp in variation of parameters?", "yp = u1 y1 + u2 y2"},
                {"What are the formulas for u1' and u2'?", "u1' = -y2 g / W and u2' = y1 g / W"},
                {"What must be true of the DE before applying these formulas?", "It must be in standard form (coefficient of y'' equals 1) so that g(x) is the right-hand side"},
                {"When is variation of parameters preferred?", "When g(x) is not suited to undetermined coefficients, e.g. tan x, sec x, ln x, 1/x"},
                {"What are the steps of variation of parameters?", "Find yc; compute W; compute u1' and u2'; integrate; form yp; write y = yc + yp"},
                {"Find yp for y'' + y = sec x", "yp = x sin x + cos x ln|cos x|"}
            }},
            {"Laplace Transformation", {
                {"Define the Laplace transform of f(t)", "L{f(t)} = int from 0 to infinity of e^(-st) f(t) dt = F(s)"},
                {"L{1} = ?", "1/s"},
                {"L{t^n} = ?", "n! / s^(n+1)"},
                {"L{e^(at)} = ?", "1 / (s - a)"},
                {"L{sin kt} = ?", "k / (s^2 + k^2)"},
                {"L{cos kt} = ?", "s / (s^2 + k^2)"},
                {"L{sinh kt} = ?", "k / (s^2 - k^2)"},
                {"L{cosh kt} = ?", "s / (s^2 - k^2)"},
                {"L{f'(t)} = ?", "sF(s) - f(0)"},
                {"L{f''(t)} = ?", "s^2 F(s) - s f(0) - f'(0)"},
                {"State the first shifting theorem", "L{e^(at) f(t)} = F(s - a)"},
                {"L{t f(t)} = ?", "-F'(s)"}
            }},
            {"Inverse Laplace Transformation", {
                {"L^-1{1/s} = ?", "1"},
                {"L^-1{1/s^(n+1)} = ?", "t^n / n!"},
                {"L^-1{1/(s - a)} = ?", "e^(at)"},
                {"L^-1{k/(s^2 + k^2)} = ?", "sin kt"},
                {"L^-1{s/(s^2 + k^2)} = ?", "cos kt"},
                {"State the inverse first shifting theorem", "L^-1{F(s - a)} = e^(at) f(t)"},
                {"What techniques help find inverse transforms?", "Partial fractions and completing the square"},
                {"Find L^-1{1/(s^2 + 4s + 5)}", "e^(-2t) sin t"},
                {"Find L^-1{1/(s(s + 1))}", "1 - e^(-t)"},
                {"What are the steps to solve an IVP using Laplace transforms?", "Transform both sides; solve for Y(s); apply the inverse transform to get y(t)"}
            }},
            {"Heaviside Functions", {
                {"Define the Heaviside (unit step) function u(t - a)", "u(t - a) = 0 for t < a and 1 for t >= a"},
                {"L{u(t - a)} = ?", "e^(-as) / s"},
                {"State the second shifting theorem", "L{f(t - a) u(t - a)} = e^(-as) F(s)"},
                {"What is the inverse form of the second shifting theorem?", "L^-1{e^(-as) F(s)} = f(t - a) u(t - a)"},
                {"Write g(t) for t < a and h(t) for t >= a using unit steps", "g(t) + [h(t) - g(t)] u(t - a)"},
                {"Write a pulse equal to 1 on a <= t < b using unit steps", "u(t - a) - u(t - b)"},
                {"How do you transform g(t) u(t - a)?", "Rewrite g(t) in terms of (t - a), then apply the second shifting theorem"},
                {"Find L{u(t - 2)}", "e^(-2s) / s"}
            }},
            {"Applications of Higher-Order Differential Equations", {
                {"What is Hooke's Law?", "F = kx (restoring force is proportional to displacement)"},
                {"What is the DE for a spring-mass system?", "m x'' + c x' + k x = f(t)"},
                {"What is the DE for undamped free motion, and its natural frequency?", "x'' + w^2 x = 0, with w = sqrt(k/m)"},
                {"What is the solution of undamped free motion?", "x = c1 cos wt + c2 sin wt"},
                {"What are the period and frequency of undamped motion?", "Period T = 2 pi / w. Frequency f = w / (2 pi)"},
                {"When is a damped system overdamped, critically damped, underdamped?", "c^2 - 4mk > 0, c^2 - 4mk = 0, c^2 - 4mk < 0 respectively"},
                {"What is resonance?", "When the driving frequency equals the natural frequency, causing amplitude to grow"},
                {"What is the DE of a series LRC circuit?", "L q'' + R q' + q/C = E(t)"}
            }}
        };
    }

    const std::vector<Topic>& getTopicsForTerm(std::string term) {
        if (term == "prelim") return prelimTopics;
        if (term == "midterm") return midtermTopics;
        return finalsTopics;
    }

public:
    DiffEqReviewer() : currentIndex(0) {
        srand(time(0));
        initData();
    }

    int getTopicCount(std::string term) {
        return (int)getTopicsForTerm(term).size();
    }

    std::string getTopicName(std::string term, int topicIndex) {
        const auto& topics = getTopicsForTerm(term);
        if (topicIndex >= 0 && topicIndex < (int)topics.size()) {
            return topics[topicIndex].name;
        }
        return "";
    }

    void loadTopic(std::string term, int topicIndex) {
        currentDeck.clear();
        currentIndex = 0;
        const auto& topics = getTopicsForTerm(term);

        if (topicIndex == -1) {
            // All Topics
            for (const auto& t : topics) {
                currentDeck.insert(currentDeck.end(), t.cards.begin(), t.cards.end());
            }
        } else if (topicIndex >= 0 && topicIndex < (int)topics.size()) {
            currentDeck = topics[topicIndex].cards;
        }

        // Shuffle deck
        for (int i = (int)currentDeck.size() - 1; i > 0; --i) {
            int j = rand() % (i + 1);
            std::swap(currentDeck[i], currentDeck[j]);
        }
    }

    std::string getQuestion() {
        if (currentDeck.empty()) return "Select a term and topic to start.";
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
        .function("getTopicCount", &DiffEqReviewer::getTopicCount)
        .function("getTopicName", &DiffEqReviewer::getTopicName)
        .function("loadTopic", &DiffEqReviewer::loadTopic)
        .function("getQuestion", &DiffEqReviewer::getQuestion)
        .function("getAnswer", &DiffEqReviewer::getAnswer)
        .function("getCardIndex", &DiffEqReviewer::getCardIndex)
        .function("getTotalCards", &DiffEqReviewer::getTotalCards)
        .function("nextCard", &DiffEqReviewer::nextCard);
}
