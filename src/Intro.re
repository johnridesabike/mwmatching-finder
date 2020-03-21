open Belt;
let graph = SampleData.a;
let vertices = Graph.verticesToArray(graph);
let editVertex = _ => ();
let editEdge = (_, _, _) => ();

[@react.component]
let make = (~close) => {
  let mates = Graph.Mates.useMates(graph);
  <article className="intro">
    <h2> "Welcome to the maximum weighted matching finder"->React.string </h2>
    <p>
      {|This is an app for finding matches on weighted graphs. If you're new
       to this topic, here's a quick introduction.|}
      ->React.string
    </p>
    <p style=Css.(style([textAlign(`center)]))>
      <button className="font-small" onClick={_ => close()}>
        "Skip this and take me to the app."->React.string
      </button>
    </p>
    <p>
      "Suppose you have a list of chess players: "->React.string
      <em>
        "Mary, Joseph, Matthew, Mark, Luke, John, Peter, Andrew, James,"
        ->React.string
      </em>
      " and "->React.string
      <em> "Philip"->React.string </em>
      ", and you want to pair them to compete in a tournament round."
      ->React.string
    </p>
    <p>
      "Your first step is to list all of your possible pairings."->React.string
    </p>
    <ul>
      {[|
         "Mary, Joseph",
         "Mary, Matthew",
         "Joseph, Matthew",
         "Joseph, Mark",
         "Matthew, Luke",
         "Mark, Luke",
         "Mary, Andrew",
         "Luke, Peter",
         "Peter, John",
         "Andrew, Philip",
         "Mark, James",
       |]
       ->Array.map(s => <li key=s> s->React.string </li>)
       ->React.array}
    </ul>
    <p>
      "(This isn't "->React.string
      <em> "all"->React.string </em>
      {| possible pairings, but for brevity we'll stick to this list.
       In practice, your list will usually be much longer.)|}
      ->React.string
    </p>
    <p>
      "Next, you will need to determine the "->React.string
      <em> "weight"->React.string </em>
      {| of each pairing. This is a floating-point number that indicates how
       desirable that pairing is.|}
      ->React.string
    </p>
    <table style=Css.(style([margin(`auto), borderCollapse(`collapse)]))>
      <tbody>
        <tr>
          <th colSpan=2 scope="col"> "Pair"->React.string </th>
          <th scope="col"> "Weight"->React.string </th>
        </tr>
        {[|
           ("Mary", "Joseph", 40.),
           ("Mary", "Michael", 40.),
           ("Joseph", "Michael", 60.),
           ("Joseph", "Gabriel", 55.),
           ("Michael", "Raphael", 55.),
           ("Gabriel", "Raphael", 50.),
           ("Mary", "Paul", 15.),
           ("Raphael", "Peter", 30.),
           ("Peter", "John", 10.),
           ("Paul", "James", 10.),
           ("Gabriel", "Andrew", 30.),
         |]
         ->Array.map(((i, j, w)) =>
             <tr key={i ++ "+" ++ j}>
               <td style=Css.(style([padding2(~v=px(4), ~h=px(8))]))>
                 i->React.string
               </td>
               <td style=Css.(style([padding2(~v=px(4), ~h=px(8))]))>
                 j->React.string
               </td>
               <td style=Css.(style([padding2(~v=px(4), ~h=px(8))]))>
                 {w->Js.String.make->React.string}
               </td>
             </tr>
           )
         ->React.array}
      </tbody>
    </table>
    <p>
      "Alternatively, we can lay it out on a cross-table like this:"
      ->React.string
    </p>
    <div style=Css.(style([overflowX(`scroll)]))>
      <GraphTable.Table
        graph
        vertices
        mates=Graph.Mates.empty
        editEdge
        editVertex
        disabled=true
        style=Css.(style([margin(`auto)]))
      />
    </div>
    <p>
      {|In graph theory, each of the people is a "vertex," and the
        connection between each pair of people is an "edge." Another way
        to visualize it is like this:|}
      ->React.string
    </p>
    <ForceGraph
      graph
      mates=Graph.Mates.empty
      width=400
      height=400
      style=Css.(style([margin(`auto)]))>
      "Tap or mouse-over a circle to see who it represents."->React.string
    </ForceGraph>
    <p>
      {|Now we let the algorithm work its magic. It will find a series of
        edges that have the highest possible combined weight, where no
        vertex is listed twice, and where no vertex is without a mate. In
        this example, the result is:|}
      ->React.string
    </p>
    <ul>
      {[|
         "Mary, Joseph",
         "Matthew, Luke",
         "Mark, James",
         "John, Peter",
         "Andrew, Philip",
       |]
       ->Array.map(s => <li key=s> s->React.string </li>)
       ->React.array}
    </ul>
    <p> "Or when we visualize it:"->React.string </p>
    <ForceGraph
      graph mates height=400 width=400 style=Css.(style([margin(`auto)]))>
      "Tap or mouse-over a circle to see who it represents."->React.string
    </ForceGraph>
    <p>
      {|Note that we couldn't use the edge with the highest weight because
        choosing it would leave another vertex with no connections. We also
        have to use the two edges with the lowest weights because we're
        committed to matching as many vertices as possible.|}
      ->React.string
    </p>
    <p>
      {|As you can see, finding the maximum weighted matching is often
        unintuitive. Imagine how much more difficult this becomes when you
        have dozens, or hundreds, of people, and we could potentially match
        every person with anyone else!|}
      ->React.string
    </p>
    <p>
      {|Fortunately, the algorithm here figures it out for us almost
        instantly. Try it out with some different data! It's powered by |}
      ->React.string
      <a href="https://github.com/johnridesabike/re-blossom" target="_blank">
        "this implementation of the blossom algorithm"->React.string
      </a>
      "."->React.string
    </p>
    <p style=Css.(style([textAlign(`center)]))>
      <button onClick={_ => close()}> "Go to app"->React.string </button>
    </p>
  </article>;
};
