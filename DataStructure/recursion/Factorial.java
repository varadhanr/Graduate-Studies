/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author varadhan
 */
public class Factorial {
    public static void main(String[] args) {
        System.out.println(new Factorial().factorial(10));
    }
    
    public long factorial(int i){
        if(i == 1)
            return 1;
        else return i * factorial(i-1);
    }
}
