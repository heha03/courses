package RSA;

import java.math.BigInteger;
import java.security.SecureRandom;
import java.util.BitSet;
import java.util.Random;

import javax.rmi.CORBA.Util;
//������ٳ�
class M{
	public static BigInteger expmod(BigInteger base,BigInteger exp,BigInteger n){
		BigInteger ret=BigInteger.ONE;
		while(exp.signum()>0){
			if(exp.testBit(0))ret=ret.multiply(base).mod(n);
			base=base.multiply(base).mod(n);
			exp=exp.shiftRight(1);
		}return ret;
	}
}
class Utils {

    private static Random ran=null;
    private static int trytime;
    public static void setTrytime(int x){
    	if(x>0)trytime=x;
    }
    static{
        ran=new SecureRandom();
        trytime=10;
    }

    /**
     * Miller-Rabin����
     */
    private static boolean passesMillerRabin(BigInteger n){
        BigInteger thisMinusOne=n.subtract(BigInteger.ONE);
        BigInteger m=thisMinusOne;
        int k=0;
        while(!m.testBit(0)){
            m=m.shiftRight(1);k++;
        }
        //���ѡ��a
        int a= 1;
        if(n.compareTo(BigInteger.valueOf(Integer.MAX_VALUE))<0){
            a=ran.nextInt(n.intValue()-1)+1;
        } else{
            a=ran.nextInt(Integer.MAX_VALUE-1)+1;
        }

        BigInteger b=M.expmod(new BigInteger(new Integer(a).toString()),m,n);
        if(b.equals(BigInteger.ONE))return true;
        for(int i=0;i<k-1;i++){
            if(b.equals(thisMinusOne)){
                break;
            } else{
            	b=b.multiply(b).mod(n);
            }
        }
        return false;
    }
       /* if(sizeInBits < 256){
            tryTime=27;
        } else if(sizeInBits < 512){
            tryTime=15;
        } else if(sizeInBits < 768){
            tryTime=8;
        } else if(sizeInBits < 1024){
            tryTime=4;
        } else{
            tryTime=2;
        }*/


    /**
     * ��ε�����������,�ж������n�Ƿ�Ϊ����
     */
    private static boolean isPrime(BigInteger n){
        for(int i=0;i<trytime;i++){
            if(!passesMillerRabin(n)){
                return false;
            }
        }
        return true;
    }

    /**
     * ����һ��n bit������
     */
    public static BigInteger getPrime(int bitCount){
        //�������һ��n bit�Ĵ�����
        BigInteger ret=new BigInteger(bitCount,ran);
        //���nΪż��,���һ��Ϊ����
        if(!ret.testBit(0)){
            ret=ret.setBit(0);
        }
        int i=0;
        //������������,ƽ��ֻ��Ҫ����n������,�����ҵ�һ������
        while(!isPrime(ret)){
            ret=ret.add(BigInteger.valueOf(2));i++;
            System.out.println("try prime "+i);
        }
//        System.out.println(String.format("try %d\ttimes",i));
        return ret;
    }
}
public class RSA{
	//�������һ��bitcount ���صĴ�����
	public static BigInteger randomPlaintext(int bitcount){
		BigInteger ret=BigInteger.ZERO;
		java.util.Random r=new java.util.Random();
		for(int i=0;i<bitcount;i++){
			ret=ret.shiftLeft(1);
			int id=r.nextInt();
			if(id%2==1)ret=ret.add(BigInteger.ONE);
		}return ret;
	}
	//ŷ������㷨����Ԫ
	public static BigInteger inv(BigInteger a,BigInteger b){
		BigInteger a0=a,b0=b,t0=BigInteger.ZERO,t=BigInteger.ONE;
		BigInteger q=a0.divide(b0),r=a0.subtract(q.multiply(b0));
		while(r.compareTo(BigInteger.ZERO)>0){
			BigInteger temp=t0.subtract(q.multiply(t)).mod(a);
			t0=t;t=temp;a0=b0;b0=r;q=a0.divide(b0);r=a0.subtract(q.multiply(b0));
		}
		if(!b0.equals(BigInteger.ONE))return new BigInteger("-1");
		return t;
	}
	//�õ�һ����fn���ص�����Ϊb
	public static BigInteger getB(BigInteger fn){
		BigInteger ret=new BigInteger("3");
		while(ret.compareTo(fn)<0){
			if(fn.gcd(ret).equals(BigInteger.ONE))return ret;
			ret=ret.add(new BigInteger("2"));
			System.out.println("try b");
		}return new BigInteger("-2");
	}
	//������x���м���
	public static BigInteger encrypt(BigInteger n,BigInteger b,BigInteger x){
		return M.expmod(x, b, n);
	}
	//������y���н���
	public static BigInteger decrypt(BigInteger n,BigInteger a,BigInteger y){
		return M.expmod(y, a, n);
	}
	//ʹ����ʦ�����Ӳ��Գ�����Ƶ���ȷ��
	public static void test(BigInteger p,BigInteger q,BigInteger b){
		BigInteger n=p.multiply(q),fn=p.subtract(BigInteger.ONE).multiply(q.subtract(BigInteger.ONE));
		BigInteger a=inv(fn, b);
		System.out.println("a�������Ϊ"+a);
		System.out.println("a����ȷ���Ϊ6597");
		
		BigInteger plaintext=new BigInteger("9726");
		System.out.println("���ܽ��Ϊ"+encrypt(n, b, plaintext));
		System.out.println("��ȷ���Ϊ5761");
		
		BigInteger ciphertext=new BigInteger("5761");
		System.out.println("���ܽ��Ϊ"+decrypt(n, a, ciphertext));
		System.out.println("��ȷ���Ϊ9726");
	}
	
	public static void main(String[] args){
		// TODO Auto-generated method stub
		//���Խ����ȷ��
		{
			BigInteger p=new BigInteger("101"),q=new BigInteger("113"),b=new BigInteger("3533");
			test(p, q, b);
		}
		//�趨RSA�ı���λ��
		int bitCount=256;
		//���������ޱ����Ե��ظ�������Ҫ��ȷ���ԵĻ������Ϊ26���������Ứ���൱����ʱ�����
		//�����˾�׼��ʱ�䣬����Ϊ10��һ��ȴ�2~3min���ɵõ���������
		Utils.setTrytime(10);
		
		//����RSA��p,q,n,a,b
		BigInteger p,q;
		//�Ƿ��Զ���������
		boolean ignoreGetPrime=false;
		if(!ignoreGetPrime){
			System.out.println("get prime...please wait for several minutes\n");
			p=Utils.getPrime(bitCount);q=Utils.getPrime(bitCount);
			while(p.equals(q))q=Utils.getPrime(bitCount);		
		}else{
			//ʹ��Ԥ�ȵõ����������е���
			p=new BigInteger("8934988777623417856567003555675677352545782113481241161447081632644259540271");
			q=new BigInteger("26832046253915928350733184174035429010276427232001718253289431619878114723191");
		}
		
		BigInteger n=p.multiply(q);
		BigInteger fn=p.subtract(BigInteger.ONE).multiply(q.subtract(BigInteger.ONE));
		BigInteger b=getB(fn);
		if(b.equals(new BigInteger("-2"))){
			System.out.println("error!,please restart this programe");
			return;
		}
		BigInteger a=inv(fn, b);
		//�����Կ��˽Կ
		System.out.println("public key :\nn: "+n+"\nb: "+b);
		System.out.println("private key :\np: "+p+"\nq: "+q+"\na: "+a);
		
		//������ɴ����������м��ܺͽ��ܲ���
		BigInteger m=randomPlaintext(bitCount);
		BigInteger em=encrypt(n, b, m),dm=decrypt(n, a, em);
		System.out.println("the plaintext is "+m+"\nafter  encryt is +"+em+"\nafter decrypt is "+dm);
	}

}
