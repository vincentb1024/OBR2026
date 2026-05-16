int r1,g1,b1,r2,g2,b2;
int branco1,preto1,branco2,preto2;
int erro,i1,i2;
float kp = 0.5;

task main()
{

	while(true){
        getColorRGB(S1,r1,g1,b1);
        getColorRGB(S2,r2,g2,b2);
        i1 = 100*(r1-preto1)/(branco1-preto1);
        i2 = 100*(r2 - preto2)/(branco2-preto2);
        erro = i1 - i2;
        motor[motorA] = 30 + erro*kp;
        motor[motorB] = 30 - erro*kp;
    }
}
