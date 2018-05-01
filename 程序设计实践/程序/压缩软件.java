package four;

import java.awt.geom.Ellipse2D;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Dictionary;
import java.util.HashMap;
import java.util.HashSet;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Scanner;

import javax.print.attribute.standard.MediaSize.Other;
import javax.rmi.ssl.SslRMIClientSocketFactory;
import javax.swing.plaf.SliderUI;
class Debug{
	public String fileName;
	public Debug(String fileName)throws Exception{
		this.fileName=fileName;
		byte[]in=new byte[10];
		int cnt=0;
		in[cnt++]=1;in[cnt++]=2;in[cnt++]=3;in[cnt++]=4;
		in[cnt++]=5;in[cnt++]=5;
		in[cnt++]=6;in[cnt++]=6;in[cnt++]=6;in[cnt++]=6;
//		byte []in=new byte[16];
//		int cnt=0;
//		in[cnt++]=1;in[cnt++]=0;in[cnt++]=3;in[cnt++]=4;in[cnt++]=5;in[cnt++]=6;
//		in[cnt++]=0;in[cnt++]=3;in[cnt++]=4;in[cnt++]=5;in[cnt++]=6;
//		in[cnt++]=6;in[cnt++]=0;in[cnt++]=3;in[cnt++]=4;in[cnt++]=5;
		FileOutputStream fos=new FileOutputStream(new File(fileName));
		fos.write(in);fos.close();
	}
}
class TNode{
	public int id,val,fre,lid,rid;
	public TNode(int id,int val,int fre,int lid,int rid){
		this.id=id;this.val=val;this.fre=fre;this.lid=lid;this.rid=rid;
	}
}
class Huffman{
	public static final int maxn=257;
	public static final int maxin=1024*1024;
	
	public String maps[]=new String[maxn];
	public int in[];
	
	public String fileName;
	
	public Huffman(String fileName){
		this.fileName=fileName;
	}
	public String suf;
	public String getSuf(){
		return fileName.substring(fileName.lastIndexOf('.')+1);
	}
	public String compressName(){///.txt to .huf
		return fileName.substring(0,fileName.lastIndexOf('.'))+".huf";
	}
	public String decompressName(){///maybe the name should be saved as well
		return fileName.substring(0, fileName.lastIndexOf('.'))+"1."+suf;
//		File now=new File(fileName);
//		now=now.getParentFile();
//		File all[]=now.listFiles();
//		HashSet<String> nameAppear=new HashSet<String>();
//		for(int i=0;i<all.length;i++){
//			System.out.println(all[i].getName());
//			nameAppear.add(all[i].getName());	
//		}
//
//		
//		String begName=fileName.substring(0, fileName.lastIndexOf('.'));
//		String ret=begName+".txt";
//		if(!nameAppear.contains(ret))return ret;
//		for(Integer i=1;;i++){
//			ret=begName+"("+i.toString()+").txt";
//			if(!nameAppear.contains(ret))return ret;
//		}
	}
	
	public int cnt[]=new int[maxn];
	public int root;
	public TNode tree[]=new TNode[maxn*4];
	public void dfs(int now,String s){
		TNode t=tree[now];
		if(t.val!=-1){
			maps[t.val]=s;
			if(s.equals("")){
				maps[t.val]="0";
			}
		}
		else{
			dfs(t.lid, s+'0');dfs(t.rid, s+'1');
		}
	}
	public int buildTree(){
		Arrays.fill(cnt, 0);root=-1;
		int len=in.length;
		for(int i=0;i<len;i++){
			int ini=in[i];
//			if(ini<0)ini=ini+128*2;
//			System.out.println(ini);
			cnt[ini]++;
		}
		Comparator<TNode> tnodecmp;
		tnodecmp=new Comparator<TNode>() {
			public int compare(TNode t1,TNode t2){
				return t1.fre-t2.fre;
			}
		};
		Queue<TNode> Q=new PriorityQueue<TNode>(maxn*4,tnodecmp);
		int byteAppear=0;
		for(int i=0;i<maxn;i++)if(cnt[i]>0){
			++root;byteAppear++;
			tree[root]=new TNode(root,i,cnt[i],-1,-1);
			Q.add(tree[root]);
		}
		while(Q.size()>1){
			TNode lson=Q.poll();TNode rson=Q.poll();
			++root;
			tree[root]=new TNode(root, -1, lson.fre+rson.fre, lson.id, rson.id);
			Q.add(tree[root]);
		}
		System.out.println("root: "+root);
		dfs(root, "");
		return byteAppear;
	}
	public int byteToInt(byte x){
		if(x<0)return x+256;else return x;
	}
	public boolean b[]=new boolean[maxin*8*4];
	public int bLen;
	public void byteToBit(int x){for(int i=7;i>=0;i--)if((x&(1<<i))==0)b[bLen++]=false;else b[bLen++]=true;}
	public void stringToBit(String x){for(int i=0;i<x.length();i++)if(x.charAt(i)=='0')b[bLen++]=false;else b[bLen++]=true;}
	public void setB(int byteAppear){
		System.out.println("byteAppear: "+byteAppear);
		bLen=0;
		byteToBit((byteAppear/256));byteToBit((byteAppear%256));///first 16 is byteAppear,then each match byte-codeLen-code
		for(int i=0;i<maxn;i++)if(cnt[i]>0){
			byteToBit(i);byteToBit(maps[i].length());stringToBit(maps[i]);
		}
		int len=in.length;
		for(int i=0;i<len;i++)stringToBit(maps[in[i]]);
		
		b[bLen++]=true;
		while(bLen%8!=0)b[bLen++]=false;
	}
	public double compressRate()throws Exception{
		FileInputStream fis=new FileInputStream(new File(fileName));
		byte []bin=new byte[fis.available()];fis.read(bin);fis.close();
		
		in=new int[bin.length];
		for(int i=0;i<bin.length;i++){
			if(bin[i]<0)in[i]=bin[i]+256;else in[i]=bin[i];
		}
		
		int byteAppear=buildTree();
		int cntByte=2,cntBit=0;
		for(int i=0;i<maxn;i++)if(cnt[i]>0){
			cntByte+=2;cntBit+=maps[i].length();
		}
		int len=in.length;
		for(int i=0;i<len;i++)cntBit+=maps[in[i]].length();
		cntBit++;
		while(cntBit%8!=0)cntBit++;
		return (double)(cntBit/8+cntByte+getSuf().length()+1)/bin.length;
	}
	public byte[] setOut(){
		String suf=getSuf();
		int oLen=0;
		byte[]out=new byte[bLen/8+suf.length()+1];
		out[oLen++]=(byte)suf.length();
		for(int i=0;i<suf.length();i++)out[oLen++]=(byte)suf.charAt(i);
		for(int i=0;i<bLen;i+=8){
			int outi=0;
			for(int j=0;j<8;j++){outi<<=1;if(b[i+j])outi|=1;}
			out[i/8+oLen]=(byte)outi;
		}
		return out;
	}
	public String output(int byteAppear)throws Exception{
		///fileName needs to be modifies
		FileOutputStream fos=new FileOutputStream(new File(compressName()));
		setB(byteAppear);
		for(int i=0;i<bLen;i++)if(!b[i])System.out.printf("0");else System.out.printf("1");
		System.out.println();
		byte[] out=setOut();
		fos.write(out);fos.close();
//		for(int i=0;i<out.length;i++){
//			System.out.println(out[i]);
//		}
		System.out.println("compress ended");
//		in=out;
		return compressName();
	}
	public String compress()throws Exception{
		FileInputStream fis=new FileInputStream(new File(fileName));
		byte []bin=new byte[fis.available()];fis.read(bin);fis.close();
		
		in=new int[bin.length];
		for(int i=0;i<bin.length;i++){
//			if(bin[i]<0)in[i]=bin[i]+256;else in[i]=bin[i];
			in[i]=byteToInt(bin[i]);
		}
		
		return output(buildTree());
	}
	public boolean d[];
	public int dLen;
	public void setD(){
		suf="";
		int starti=0;
		int sufLen=in[starti++];
		for(int i=0;i<sufLen;i++)suf+=(char)((byte)in[starti++]);
		System.out.println("suf: "+suf);
		int inLen=in.length;
		d=new boolean[inLen*8];dLen=0;
		for(int i=starti;i<inLen;i++){
			for(int j=7;j>=0;j--){
				if((in[i]&(1<<j))!=0)d[dLen++]=true;else d[dLen++]=false;
			}
		}
		while(!d[dLen-1])dLen--;
		dLen--;
	}
	int bitToByte(int pos){
		int ret=0;
		for(int i=0;i<8;i++){
			ret<<=1;
			if(d[pos+i])ret|=1;
		}return ret;
	}
	public void decompress()throws Exception{
		FileInputStream fis=new FileInputStream(new File(fileName));
		byte []bin=new byte[fis.available()];fis.read(bin);fis.close();
		in=new int[bin.length];
		for(int i=0;i<bin.length;i++){
			in[i]=byteToInt(bin[i]);
		}
		
		setD();
//		System.out.println("dLen "+dLen);
//		for(int i=0;i<dLen;i++){
//			if(d[i])System.out.print("1");else System.out.print("0");
//		}System.out.println();
//		System.out.println("after setd");
		int pos=0;
		int byteAppear=bitToByte(0);pos+=8;
		byteAppear=byteAppear*256+bitToByte(pos);pos+=8;
		System.out.println("byteAppear "+byteAppear);
		HashMap<String, Byte> M=new HashMap<String,Byte>();
		for(int i=0;i<byteAppear;i++){
			byte matchB=(byte)bitToByte(pos);pos+=8;
			int codeLen=bitToByte(pos);pos+=8;
			String matchS="";
			for(int j=0;j<codeLen;j++){
				if(d[pos])matchS+='1';else matchS+='0';
				pos++;
			}
//			System.out.println(matchB+' '+matchS);
			M.put(matchS, matchB);
		}
//		System.out.println("when setOut");
		
		byte[]out=new byte[maxin];
		int outLen=0;
		while(pos<dLen){
			int nPos=pos;
			String now="";
			while(!M.containsKey(now)){
				if(nPos>=dLen){
//					System.out.println(outLen);
					FileOutputStream fos=new FileOutputStream(new File(decompressName()));
					fos.write(out, 0, outLen);fos.close();
					return;
				}
				if(d[nPos])now+='1';else now+='0';
				nPos++;
			}
			pos=nPos;
			out[outLen++]=M.get(now);
		}
		
//		for(int i=0;i<outLen;i++)System.out.println(out[i]);
		System.out.println("output only");
		FileOutputStream fos=new FileOutputStream(new File(decompressName()));
		fos.write(out, 0, outLen);fos.close();
	}
}
class LNode{
	public int beg,len;
	public String c;
	public LNode(int beg,int len,String c){
		this.beg=beg;this.len=len;this.c=c;
	}
	public void output(){
		if(len==-1)System.out.println(c);else System.out.println(beg+' '+len);
	}
}
class Lz77{///sign +char,use the char first 4 bit to record the char used to record the off,last 4 bit record the len
	public char in[];
	
	public String fileName;
	public Lz77(String fileName){
		this.fileName=fileName;
	}
	public String suf;
	public String getSuf(){
		return fileName.substring(fileName.lastIndexOf('.')+1);
	}
	public String compressName(){///.txt to .huf
		return fileName.substring(0,fileName.lastIndexOf('.'))+".lz";
	}
	public String decompressName(){///maybe the name should be saved as well
		return fileName.substring(0, fileName.lastIndexOf('.'))+"1."+suf ;
	}
	
	public ArrayList<LNode> v;
	public static final int minLen=3;
	public void setV(){
		v=new ArrayList<LNode>();
		int i=0,n=in.length;
		String sea="",buf="";
		while(i<n){
			buf+=in[i];
			while(i+1<n&&sea.indexOf(buf+in[i+1])!=-1){
				buf+=in[i+1];i++;
			}
			if(buf.length()>minLen){
				v.add(new LNode(sea.indexOf(buf), buf.length(), ""));
			}else{
				v.add(new LNode(-1, -1, buf));
			}
			sea+=buf;buf="";i++;
		}
	}
//	public byte[] output(int byteAppear){
//		///fileName needs to be modifies
//		FileOutputStream fos=new FileOutputStream(new File(compressName()));
//		System.out.println("compress ended");
////		in=out;
//		return compressName();
//	}
	public static final char sign=(char)0;
	
	public byte[] intToByte(int x){
		byte[] t=new byte[4];
		int tLen=0;
		while(x>0){
			t[tLen++]=(byte)(x%256);x/=256;
		}
		if(tLen==0)t[tLen++]=0;
		byte[] ret=new byte[tLen];
		for(int i=0;i<tLen;i++)ret[i]=t[tLen-i-1];
		return ret;
	}
	public int mergeSizes(int s1,int s2){
		return s1*16+s2;
	}
	public int getBegSize(int x){return x/16;}
	public int getLenSize(int x){return x%16;}
	public double compressRate()throws Exception{
		FileInputStream fis=new FileInputStream(new File(fileName));
		byte []bin=new byte[fis.available()];fis.read(bin);fis.close();
		for(int i=0;i<bin.length;i++)System.out.println(bin[i]);
		in=new char[bin.length];
		for(int i=0;i<bin.length;i++)in[i]=(char)bin[i];
		
//		System.out.println("before setv");
		setV();
		
		int ret=0;
		for(int i=0;i<v.size();i++){
			LNode vi=v.get(i);
			if(vi.len==-1){
				char []c=vi.c.toCharArray();
				for(int j=0;j<c.length;j++){
					if(c[j]==sign){
						ret+=2;
					}else{
						ret++;
					}
				}
			}else{
				byte[] begb=intToByte(vi.beg);byte[] lenb=intToByte(vi.len);
				ret+=1+1+begb.length+lenb.length;
			}
		}
		return (double)(ret+getSuf().length()+1)/bin.length;
	}
	public String compress()throws Exception{
		FileInputStream fis=new FileInputStream(new File(fileName));
		byte []bin=new byte[fis.available()];fis.read(bin);fis.close();
//		for(int i=0;i<bin.length;i++)System.out.println(bin[i]);
		in=new char[bin.length];
		for(int i=0;i<bin.length;i++)in[i]=(char)bin[i];
		
		System.out.println("before setv");
		setV();
//		System.out.println("v.size: "+v.size());
		
		byte[] out=new byte[bin.length*2];
		int oLen=0;
		suf=getSuf();
		out[oLen++]=(byte)suf.length();
		for(int i=0;i<suf.length();i++)out[oLen++]=(byte)suf.charAt(i);
		for(int i=0;i<v.size();i++){
			LNode vi=v.get(i);
			if(vi.len==-1){
				char []c=vi.c.toCharArray();
				for(int j=0;j<c.length;j++){
					if(c[j]==sign){
						out[oLen++]=(byte)sign;out[oLen++]=(byte)sign;
					}else{
						out[oLen++]=(byte)c[j];
					}
				}
			}else{
				byte[] begb=intToByte(vi.beg);byte[] lenb=intToByte(vi.len);
				out[oLen++]=(byte)sign;out[oLen++]=(byte)mergeSizes(begb.length, lenb.length);
				for(int j=0;j<begb.length;j++)out[oLen++]=begb[j];
				for(int j=0;j<lenb.length;j++)out[oLen++]=lenb[j];
				
//				System.out.println("last add");
//				if(i==v.size()-1){
//					for(int j=0;j<begb.length;j++)System.out.println(begb[j]);
//					for(int j=0;j<lenb.length;j++)System.out.println(lenb[j]);
//				}
			}
		}
//		for(int i=0;i<v.size();i++)v.get(i).output();
		System.out.println("oLen "+oLen);
//		for(int i=0;i<oLen;i++)System.out.println(out[i]);
		FileOutputStream fos=new FileOutputStream(new File(compressName()));
		fos.write(out, 0, oLen);fos.close();
		
		return compressName();
	}
	public int byteToInt(byte x){
		if(x<0)return x+256;else return x;
	}
	public void decompress()throws Exception{
		FileInputStream fis=new FileInputStream(new File(fileName));
		byte []bin=new byte[fis.available()];fis.read(bin);fis.close();
//		for(int i=0;i<bin.length;i++)System.out.println(bin[i]);
		
		
		String ori="";
		{
			int n=bin.length,i=0;
			int sufLen=byteToInt(bin[i++]);
			suf="";
			for(int j=0;j<sufLen;j++)suf+=(char)bin[i++];
			while(i<n){
				if((char)bin[i]!=sign)ori+=(char)bin[i++];
				else if((char)bin[i+1]==sign){
					ori+=sign;i+=2;
				}else{
					int begLen=getBegSize(byteToInt(bin[++i]));
					int lenLen=getLenSize(byteToInt(bin[i++]));
					int beg=0,len=0;
					for(int j=0;j<begLen;j++)beg=beg*256+byteToInt(bin[i++]);
					for(int j=0;j<lenLen;j++)len=len*256+byteToInt(bin[i++]);
					ori+=ori.substring(beg, beg+len);
				}
			}
		}
		
		
		char[]cout=ori.toCharArray();
		byte[]out=new byte[cout.length];
		for(int i=0;i<cout.length;i++)out[i]=(byte)cout[i];
//		int oLen=cout.length;
		FileOutputStream fos=new FileOutputStream(new File(decompressName()));
		fos.write(out, 0, out.length);fos.close();
//		for(int i=0;i<out.length;i++)System.out.println(out[i]);
	}
}

public class FourHomework {

	public static void main(String[] args)throws Exception {
		// TODO Auto-generated method stub
		Scanner cin=new Scanner(new BufferedInputStream(System.in));
		while(true){
			System.out.println("you want to compress or decompress?");
			String cd=cin.next();
			System.out.println("input the fileName with the path,please");
			String fileName=cin.next();
			File file=new File(fileName);
			if(!file.exists()||!file.isFile()){
				System.out.println("This file doesn't exist");
			}else if(cd.charAt(0)=='c'){
				Huffman huc=new Huffman(fileName);
				Lz77 lzc=new Lz77(fileName);
				double huRate=huc.compressRate();
				double lzRate=lzc.compressRate();
				System.out.println("The compress rate of Huffman is "+huRate);
				System.out.println("The compress rate od Lz77 is "+lzRate);
				
				if(huRate<lzRate)System.out.println("Huffman is better");
				else System.out.println("Lz77 is better");
				
				String order=cin.next();
				String afterCompress;
				if(order.charAt(0)=='h')afterCompress=huc.compress();
				else afterCompress=lzc.compress();
				
				System.out.println("This file has been compressed as "+afterCompress);
			}else{
				Huffman hud=new Huffman(fileName);
				Lz77 lzd=new Lz77(fileName);
				String suf=hud.getSuf();
				try{
					boolean compress=false;
					if(suf.equals("lz")){
						lzd.decompress();
						compress=true;
					}else if(suf.equals("huf")){
						hud.decompress();
						compress=true;
					}else System.out.println("Sorry ,the decompression of this file isn't supported");
					
					if(compress)System.out.println("decompression succeed");
				}catch(Exception e){
					System.out.println("Sorry, this file has been destroyed");
				}
			}
		}
		
//		Debug debug=new Debug(fileName);
//		Lz77 lzc=new Lz77(fileName);
//		System.out.println(lzc.compressSize());
//		String afterCompress=lzc.compress();
//		Lz77 lzd=new Lz77(afterCompress);
//		System.out.println("decompress begins");
//		lzd.decompress();
	}

}
