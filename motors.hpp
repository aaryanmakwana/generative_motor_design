#include "bldc.hpp"
#include "srm.hpp"
#include "induction.hpp"
#include "dc.hpp"
#include <memory>

using namespace std;

class Motor{
    struct Concept{
        virtual ~Concept() = default;

        virtual void guess_variables(struct parameters) const = 0;
        virtual void solve(struct parameters) const = 0;
        virtual void print_variables() const = 0;
        virtual void print_design() const = 0;
        virtual void print_performance() const = 0;
        virtual void print_all() const = 0;
        virtual double estimate_efficiency(struct eval_func EF) const = 0;

        virtual std::unique_ptr<Concept> clone() const=0;
    };

    template<typename T>
    struct Model : Concept{
        T data;
        Model(const T& t) : data(t) {}

        void guess_variables(struct parameters params) const override {data.guess_variables(params);}
        void solve(struct parameters params) const override {data.solve(params);}
        void print_variables() const override {data.print_variables();}
        void print_design() const override {data.print_design();}
        void print_performance() const override {data.print_performance();}
        void print_all() const override {data.print_all();}
        double estimate_efficiency(struct eval_func EF) const override {return data.estimate_efficiency(EF);}

        std::unique_ptr<Concept> clone() const override {return std::make_unique<Model<T>>(data);}
    };

    std::unique_ptr<Concept> ptr;

    public:
    template<typename T>
    Motor(const T& t) : ptr(std::make_unique<Model<T>>(t)) {}
    Motor(const Motor& other) : ptr(other.ptr->clone()) {}
    Motor(Motor&&) = default;

    void guess_variables(struct parameters params) const {ptr->guess_variables(params);}
    void solve(struct parameters params) const {ptr->solve(params);}
    void print_variables() const {ptr->print_variables();}
    void print_design() const {ptr->print_design();}
    void print_performance() const {ptr->print_performance();}
    void print_all() const {ptr->print_all();}
    double estimate_efficiency(struct eval_func EF) const {return ptr->estimate_efficiency(EF);}
};
