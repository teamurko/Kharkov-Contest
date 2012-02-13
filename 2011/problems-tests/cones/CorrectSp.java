import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;


public class CorrectSp implements Runnable {

    class Point
    {
        public Point(int x, int y) {
            this.x = x;
            this.y = y;
        }
        public int x, y;
    }

    class Cone
    {
        Cone(Point p, int r, int h) {
            this.center = p;
            this.r = r;
            this.h = h;
        }
        Point center;
        int r, h;   
    }
    
    long sqr(long x) {
        return x * x;
    }

    long dist2(Point a, Point b)
    {
        return sqr(a.x - b.x) + sqr(a.y - b.y);
    }

    boolean intersect(Cone a, Cone b)
    {
        return dist2(a.center, b.center) <= sqr(a.r + b.r);
    }

    int bfs(List<List<Integer>> graph) {
        /*
        for(int i = 0; i < graph.size(); ++i) {
            List<Integer> adj = graph.get(i);
            for(int j = 0; j < adj.size(); ++j) {
                System.out.print(adj.get(j));
                if (j < adj.size() - 1) System.out.print(" ");
            }
            System.out.println();
        }
        */
        
        final int INF = 100000;
     
        LinkedList<Integer> q = new LinkedList<Integer>();
        
        
        List<Integer> d = new ArrayList<Integer>();
        for(int i = 0; i < graph.size(); ++i) {
            d.add(INF);
        }
        
        d.set(0, 0);
        q.add(0);
        
        while (q.size() != 0) {
            int from = q.pop();
            for(int i = 0; i < graph.get(from).size(); ++i) {
                int to = graph.get(from).get(i);
                if (d.get(to) == INF) {
                    d.set(to, d.get(from) + 1);
                    q.add(to);
                }
            }    
        }
        if (d.get(d.size()-1) == INF) return -1;
        return d.get(d.size()-1);
    }

    class Frac implements Comparable<Frac>
    {
        @Override
        public int compareTo(Frac frac) {
            return BigInteger.valueOf(this.a).multiply(BigInteger.valueOf(frac.b)).compareTo(
                        BigInteger.valueOf(this.b).multiply(BigInteger.valueOf(frac.a)));
        }
        
        public Frac(long a, long b)
        {
            if (b < 0) {
                b = -b;
                a = -a;
            }
            this.a = a;
            this.b = b;
        }
        
        public long a, b;
    };
    
    boolean le(Frac f, BigInteger D) {
        if (f.a <= 0) {
            return true;
        }
        BigInteger result = BigInteger.valueOf(f.a).pow(2);
        return result.compareTo(D.multiply(BigInteger.valueOf(f.b).pow(2))) <= 0;
    }
    
    boolean ge(Frac f, BigInteger D) {
        if (f.a < 0) {
            return false;
        }
        BigInteger result = BigInteger.valueOf(f.a).pow(2);
        return result.compareTo(D.multiply(BigInteger.valueOf(f.b).pow(2))) >= 0;
    }

    //TODO
    boolean visible(Cone one, Cone two, Cone middle)
    {
        Frac a = new Frac(0, 1);
        Frac b = new Frac(1, 1);
        long y1 = one.center.y, x1 = one.center.x, z1 = one.h;
        long y2 = two.center.y, x2 = two.center.x, z2 = two.h;
        long y0 = middle.center.y, x0 = middle.center.x, h = middle.h, r = middle.r;

        if (z1 == z2) {
            if (middle.h < z1) return true;
        }
        else {
            //z1 + (z2 - z1) * t <= h
            if (z1 > z2) {
                if (a.compareTo(new Frac(z1 - h, z1 - z2)) < 0) {    
                    a = new Frac(z1 - h, z1 - z2);
                }
            }
            else {
                if (new Frac(h - z1, z2 - z1).compareTo(b) < 0) {
                    b = new Frac(h - z1, z2 - z1);
                }
            }
        }
        if (b.compareTo(a) < 0) return true;

        //z(t) = z1 + (z2 - z1) * t;
        //y(t) = y1 + (y2 - y1) * t;
        //x(t) = x1 + (x2 - x1) * t;
        //r(t) = r * (h - z(t)) / h;
        //(x(t) - x0) ^ 2 + (y(t) - y0) ^ 2 <= r^2(t);
        //((x2-x1)*t + x1-x0)^2 + ((y2-y1)*t + y1-y0)^2 <= r*r/h/h*(h-z1 - (z2-z1)*t)^2


        //sqr(x2-x1)t^2*h^2 + 2(x1-x0)*(x2-x1)t*h^2 + sqr(x1-x0)*h^2 + 
        //sqr(y2-y1)t^2*h^2 + 2(y1-y0)*(y2-y1)t*h^2 + sqr(y1-y0)*h^2 <=
        //r^2(sqr(h-z1) - 2(h-z1)(z2-z1)t + sqr(z2-z1)t^2)
        
        long A = sqr(x2 - x1) * h * h + sqr(y2 - y1) * h * h - sqr(z2-z1) * r * r;
        long B = 2 * (x1 - x0) * (x2 - x1) * h * h + 2 * (y1 - y0) * (y2 - y1) * h * h +
            2 * (h - z1) * (z2 - z1) * r * r;
        long C = sqr(x1 - x0) * h * h + sqr(y1 - y0) * h * h - sqr(h - z1) * r * r;
        // A * t^2 + B * t + C <= 0, t in [a, b] (1)
        // A == 0
        if (A == 0) {
            if (B == 0) {
                return C != 0;
            }
            if (B < 0) {
                Frac f = new Frac(-C,-B);
                return b.compareTo(f) < 0;
            }
            else {
                Frac f = new Frac(C, B);
                return f.compareTo(a) < 0;
            }
        }

        // -B +- sqrt(B * B - 4 * A * C) = -B/2/A +- sqrt(B/A*B/A - 4 * C / A)

        BigInteger D = BigInteger.valueOf(B).pow(2).subtract(BigInteger.valueOf(4*A).multiply(BigInteger.valueOf(C)));
        // If (1) doesn't have roots, then return true iff A > 0 
        if (D.compareTo(BigInteger.ZERO) < 0) {
            return A > 0;
        }    
        // Let r1 and r2 are roots (1) (r1 <= r2).
        // Check if r1 or r2 in [a, b] or a or b satisfy (1)
        if (A * a.a * a.a + B * a.a * a.b + C * a.b * a.b <= 0) {   
            return false;
        }
        if (A * b.a * b.a + B * b.a * b.b + C * b.b * b.b <= 0) {
            return false;
        }
        if (A < 0) {
            A = -A;
            B = -B;
            C = -C;
        }
        //root1 = (-B + sqrt(D)) / 2 /A
        //a <= root1 <= b
        // b.b * a.a * 2 * A <= a.b * b.b * (-B + sqrt(D)) <= b.a * a.b * 2 * A
        // a.a / a.b * A * 2 + B <= sqrt(D)
        // b.a / b.b * A * 2 + B >= sqrt(D)
        if (le(new Frac(a.a * 2 * A + B * a.b, a.b), D)
                && ge(new Frac(b.a * 2 * A + B * b.b, b.b), D)) {
            return false;
        }
        //a.a / a.b * 2 * A <= -B - sqrt(D)
        //b.a / b.b * 2 * A >= -B - sqrt(D)
        if (ge(new Frac(-B * a.b - a.a * 2 * A, a.b), D)
                && le(new Frac(-B * b.b - b.a * 2 * A, b.b), D)) {
            return false;
        }
        return true;
    }

    boolean visible(List<Cone> cones, int one, int two)
    {
        for(int i = 0; i < cones.size(); ++i) {
            if (i == one || i == two) continue;
            if (!visible(cones.get(one), cones.get(two), cones.get(i))) return false;
        }
        return true;
    }

    void buildGraph(List<Cone> cones, List<List<Integer>> graph)
    {
        for(int i = 0; i < cones.size(); ++i) {
            graph.add(new ArrayList<Integer>());
        }
        for(int two = 0; two < graph.size(); ++two) {
            for(int one = 0; one < two; ++one) {
                if (visible(cones, one, two)) {
                    graph.get(one).add(two);
                    graph.get(two).add(one);                
                }            
            }
        }    
    }
    
    Scanner in;

    void solve()
    {
        in = new Scanner(System.in);
        int numCones = in.nextInt();
        List<Cone> cones = new ArrayList<Cone>();
        for(int i = 0; i < numCones; ++i) {
            int x = in.nextInt();
            int y = in.nextInt();
            int r = in.nextInt();
            int h = in.nextInt();
            cones.add(i, new Cone(new Point(x, y), r, h));
        }
        List<List<Integer>> graph = new ArrayList<List<Integer>>();
        buildGraph(cones, graph);
        System.out.println(bfs(graph));
    }
    
    public static void main(String[] args) {
        new Thread(new CorrectSp()).start();
    }
    
    public void run() {
        try {
            solve();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
