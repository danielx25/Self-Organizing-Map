#ifndef CRITERIOSUPERVISION_H
#define CRITERIOSUPERVISION_H


class CriterioSupervision
{
    public:
        CriterioSupervision() {}
        virtual ~CriterioSupervision() {}

        static bool criterioMp10(double *entrada, double *neurona, int &subclase)
        {
            int mp10_entrada = entrada[Configuracion::NUMERO_ENTRADAS-1]*800;
            int mp10_neurona = neurona[Configuracion::NUMERO_ENTRADAS-1]*800;
            int rangos[16] = {50, 100, 150, 183, 216, 250, 283, 316, 350, 400, 450, 500, 550, 600, std::numeric_limits<int>::infinity()};
            int subclaseEntrada = -1;
            int subclaseNeurona = -1;


            for(int i=0; i < 15; i++)
            {
                if(rangos[i] < mp10_entrada < rangos[i+1])
                {
                    subclaseEntrada = i;
                    subclase = i;
                }

                if(rangos[i] < mp10_neurona < rangos[i+1])
                subclaseNeurona = i;
            }

            if(subclaseEntrada == subclaseNeurona)
                return true;
            return false;
        }

    protected:
    private:
};

#endif // CRITERIOSUPERVISION_H
