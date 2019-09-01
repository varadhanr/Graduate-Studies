/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author varadhan
 */
public class Fibonacci {
    public static void main(String[] args) {
        System.out.println(new Fibonacci().fibo(10000));
    }
    
    public double fibo(double i){
        if(i ==0)
            return 0;
        if(i == 1)
            return 1;
        else return fibo(i -1) + fibo(i - 2);
        
    }
}
