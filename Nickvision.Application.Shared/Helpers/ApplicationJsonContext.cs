using Nickvision.Application.Shared.Models;
using System.Text.Json.Serialization;

namespace Nickvision.Application.Shared.Helpers;

[JsonSourceGenerationOptions(DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull, WriteIndented = true)]
[JsonSerializable(typeof(Configuration))]
public partial class ApplicationJsonContext : JsonSerializerContext
{

}
