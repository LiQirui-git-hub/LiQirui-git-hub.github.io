#include <bits/stdc++.h>
using namespace std;
struct Edge {
    int to;
    int weight;
    explicit Edge(int t,int w=1):to(t),weight(w) {}
};
class Graph {
private:
    vector<vector<Edge>> adj;
    int vertexCount;
public:
    explicit Graph(int n):vertexCount(n) {
        adj.resize(n);
    }
    void addDirectedEdge(int from,int to,int weight=1) {
        if (from>=0 && from<vertexCount && to>=0 && to<vertexCount) {
            adj[from].emplace_back(to,weight);
        }
    }
    void addUndirectedEdge(int u,int v,int weight=1) {
        addDirectedEdge(u,v,weight);
        addDirectedEdge(v,u,weight);
    }
    vector<vector<int>> floydWarshall() {
        vector<vector<int>> dist(vertexCount,vector<int>(vertexCount,INT_MAX));
        for (int i=0;i<vertexCount;i++) dist[i][i]=0;
        for (int i=0;i<vertexCount;i++) {
            for (const auto& edge:adj[i]) dist[i][edge.to]=edge.weight;
        }
        for (int k=0;k<vertexCount;k++) {
            for (int i=0;i<vertexCount;i++) {
                for (int j=0;j<vertexCount;j++) {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                        if (dist[i][j] > dist[i][k] + dist[k][j]) {
                            dist[i][j]=dist[i][k] + dist[k][j];
                        }
                    }
                }
            }
        }
        return dist;
    }
    vector<int> bfs(int start) {
        vector<int> result;
        vector<bool> visited(vertexCount,false);
        queue<int> q;
        q.push(start);
        visited[start]=true;
        while (!q.empty()) {
            int current=q.front();
            q.pop();
            result.push_back(current);
            for (const auto& edge:adj[current]) {
                if (!visited[edge.to]) {
                    visited[edge.to]=true;
                    q.push(edge.to);
                }
            }
        }
        return result;
    }
    vector<int> dfs(int start) {
        vector<int> result;
        vector<bool> visited(vertexCount,false);
        dfsHelper(start,visited,result);
        return result;
    }
    vector<int> dijkstra(int start) {
        vector<int> dist(vertexCount,INT_MAX);
        priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
        dist[start]=0;
        pq.push({0,start});
        while (!pq.empty()) {
            int currentDist=pq.top().first;
            int current=pq.top().second;
            pq.pop();
            if (currentDist>dist[current]) continue;
            for (const auto& edge:adj[current]) {
                int newDist=currentDist + edge.weight;
                if (newDist<dist[edge.to]) {
                    dist[edge.to]=newDist;
                    pq.push({newDist,edge.to});
                }
            }
        }
        return dist;
    }
    bool hasCycle() {
        vector<int> state(vertexCount,0);
        for (int i=0;i<vertexCount;i++) {
            if (state[i]==0 && hasCycleDFS(i,state)) {
                return true;
            }
        }
        return false;
    }
    vector<int> topologicalSort() {
        vector<int> inDegree(vertexCount,0);
        vector<int> result;
        queue<int> q;
        for (int i=0;i<vertexCount;i++) {
            for (const auto& edge:adj[i]) {
                inDegree[edge.to]++;
            }
        }
        for (int i=0;i<vertexCount;i++) {
            if (inDegree[i]==0) {
                q.push(i);
            }
        }
        while (!q.empty()) {
            int current=q.front();
            q.pop();
            result.push_back(current);
            for (const auto& edge:adj[current]) {
                inDegree[edge.to]--;
                if (inDegree[edge.to]==0) {
                    q.push(edge.to);
                }
            }
        }
        return result;
    }
    const vector<vector<Edge>>& getAdjacencyList() const {
        return adj;
    }
    int getVertexCount() const {
        return vertexCount;
    }
    void printGraph() {
        for (int i=0;i<vertexCount;i++) {
            cout << "Vertex " << i << ": ";
            for (const auto& edge:adj[i]) {
                cout << "-> " << edge.to << "(" << edge.weight << ") ";
            }
            cout << endl;
        }
    }
    vector<vector<int>> getStronglyConnectedComponents() {
        vector<vector<int>> scc;
        vector<bool> visited(vertexCount,false);
        stack<int> st;
        for (int i=0;i<vertexCount;i++) {
            if (!visited[i]) dfsForSCC(i,visited,st);
        }
        Graph transposed=getTranspose();
        fill(visited.begin(),visited.end(),false);
        while (!st.empty()) {
            int node=st.top();
            st.pop();
            if (!visited[node]) {
                vector<int> component;
                transposed.dfsForSCCCollect(node,visited,component);
                scc.push_back(component);
            }
        }
        return scc;
    }
    Graph getTranspose() const {
        Graph transposed(vertexCount);
        for (int i=0;i<vertexCount;i++) {
            for (const auto& edge : adj[i]) {
                transposed.addDirectedEdge(edge.to,i,edge.weight);
            }
        }
        return transposed;
    }
    void dfsForSCC(int node,vector<bool>& visited,stack<int>& st) {
        visited[node]=true;
        for (const auto& edge:adj[node]) {
            if (!visited[edge.to]) {
                dfsForSCC(edge.to,visited,st);
            }
        }
        st.push(node);
    }
    void dfsForSCCCollect(int node,vector<bool>& visited,vector<int>& component) {
        visited[node]=true;
        component.push_back(node);
        for (const auto& edge:adj[node]) {
            if (!visited[edge.to]) {
                dfsForSCCCollect(edge.to,visited,component);
            }
        }
    }
    vector<Edge> primMST(int start = 0) {
        vector<Edge> mstEdges;
        vector<bool> inMST(vertexCount,false);
        vector<int> key(vertexCount,INT_MAX);
        vector<int> parent(vertexCount,-1);
        priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
        key[start] = 0;
        pq.push({0,start});
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            if (inMST[u]) continue;
            inMST[u] = true;
            if (parent[u] != -1) {
                mstEdges.emplace_back(u,key[u]);
            }
            for (const auto& edge:adj[u]) {
                int v = edge.to;
                int weight = edge.weight;
                if (!inMST[v] && weight<key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                    pq.push({key[v],v});
                }
            }
        }
        return mstEdges;
    }
    int primMSTWeight(int start = 0) {
        vector<Edge> mst = primMST(start);
        int totalWeight = 0;
        for (const auto& edge:mst) {
            totalWeight += edge.weight;
        }
        return totalWeight;
    }
private:
    void dfsHelper(int node,vector<bool>& visited,vector<int>& result) {
        visited[node]=true;
        result.push_back(node);

        for (const auto& edge:adj[node]) {
            if (!visited[edge.to]) {
                dfsHelper(edge.to,visited,result);
            }
        }
    }
    bool hasCycleDFS(int node,vector<int>& state) {
        state[node]=1;
        for (const auto& edge:adj[node]) {
            if (state[edge.to]==1) {
                return true;
            }
            if (state[edge.to]==0 && hasCycleDFS(edge.to,state)) {
                return true;
            }
        }
        state[node]=2;
        return false;
    }
};