const fs = require('fs');
const path = require('path');
const matter = require('gray-matter'); // 用于解析Markdown的Front Matter

// 博客目录路径
const BLOG_DIR = path.join(__dirname, 'blog');
// 生成的索引文件路径
const INDEX_FILE = path.join(__dirname, 'posts.json');

// 确保blog目录存在
if (!fs.existsSync(BLOG_DIR)) {
    fs.mkdirSync(BLOG_DIR);
    console.log('已创建blog目录，请将Markdown文章放入该目录');
}

// 读取blog目录下的所有Markdown文件
function generatePostsIndex() {
    try {
        const files = fs.readdirSync(BLOG_DIR).filter(file => 
            file.endsWith('.md') || file.endsWith('.markdown')
        );

        const posts = [];

        files.forEach(file => {
            const filePath = path.join(BLOG_DIR, file);
            const fileContent = fs.readFileSync(filePath, 'utf8');
            
            // 解析Markdown头部的元数据（Front Matter）
            const { data } = matter(fileContent);
            
            // 提取文件名作为slug（不含扩展名）
            const slug = path.basename(file, path.extname(file));
            
            // 验证必要的元数据
            if (!data.title) {
                console.warn(`警告：文件 ${file} 缺少title元数据`);
                return;
            }
            
            // 收集文章信息
            posts.push({
                slug: slug,
                title: data.title || '无标题',
                date: data.date || new Date(fs.statSync(filePath).birthtime).toISOString().split('T')[0],
                category: data.category || '未分类',
                coverImage: data.coverImage || `https://picsum.photos/id/${Math.floor(Math.random() * 100)}/600/400`,
                contentFile: `blog/${file}`
            });
        });

        // 按日期排序（最新的在前）
        posts.sort((a, b) => new Date(b.date) - new Date(a.date));

        // 生成索引文件
        fs.writeFileSync(INDEX_FILE, JSON.stringify(posts, null, 2));
        console.log(`成功生成索引：${posts.length} 篇文章`);
    } catch (error) {
        console.error('生成索引失败：', error);
    }
}

// 执行生成
generatePostsIndex();
    