/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author varadhan
 */
public class isPalindrome  {

    public static void main(String[] args) {
        System.out.println(new isPalindrome().isPalin("QWERTYYTREWQ"));
    }
    
    public boolean isPalin(String inputStr){
        return isPalin(inputStr, 0, inputStr.length() -1);
    }
    public boolean isPalin(String inputString,int i, int j){
        if(i > j-1)
            return true;
        if(inputString.charAt(i) != inputString.charAt(j))
            return false;
        else return isPalin(inputString, i+1, j-1);
    }

}
